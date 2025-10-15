#include "platform.h"

#include "stm32l4xx_hal.h"

#include "bsp_drivers.h"
#include "peripherals/peripherals.h"
#include "drivers.h"
#include "io/io.h"

#include "data_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <wifi.h>
#include <string.h>

// Function declarations
int WIFI_ReadLine(uint8_t socket, char *string);
int WIFI_SendLine(uint8_t socket, const char *string);
int WIFI_SendHTTPRequest(uint8_t socket, const char *method, const char *path, const char *host, const char *data);
int getDate(void);
int postData(float btemp, float bhumidity, float bpressure, float temp, float pulse, float ox, int falling);
int detectFall(void);
// Configuration du réseau Wi-Fi
#define WLAN_SSID "TP-IOT"
#define WLAN_PASSWORD "azerty123!$"

// Configuration des fonctionnalités
#define WLAN_ENABLE 0
#define NFC_ENABLE 1

// Configuration du serveur distant
const uint8_t RemoteIP[] = {192, 168, 0, 254};
const uint16_t RemotePort = 8080;
// Flags de l'application
struct _AppFlags_t
{
	uint8_t sensor_update;
	uint8_t data_tx;
	uint8_t max3010x_data_ready;
	uint8_t uart_print;
	uint8_t user_button_pressed;
	uint8_t falling;
} AppFlags = {0};

// Timers
struct _AppTimer_t
{
	unsigned int delayInMs;
	uint8_t *flagPtr;
} AppTimers[] = {
	// Intervalle (ms) Flag
	{100, &AppFlags.sensor_update},
	{10000, &AppFlags.data_tx},
	{1000,
	 &AppFlags.uart_print}};
float gyroData[3];
int16_t magnetoData[3];
int16_t acceleroData[3];
int16_t acceleroDataBase[3];
int16_t sumAccelero;
int16_t sumAcceleroBase;
uint8_t ip[4];
uint8_t mac[6];
uint32_t socket = 0;
uint8_t pdata = 3;
uint16_t bytesSent;
uint16_t bytesReceived;
uint32_t time_sensor_update = 0;
uint32_t time_max3010x_calc = 0;
uint32_t time_network_tx = 0;
uint32_t time_busy_total = 0;
uint32_t time_last_second = 0;

int main(void)
{
	HAL_Init();

	// Peripherals initialization
	Clock_Init();
	GPIO_Init();
	I2C1_Init();
	UART1_Init();
	// Drivers initialization
	BSP_TSENSOR_Init();
	BSP_GYRO_Init();
	BSP_ACCELERO_Init();
	BSP_HSENSOR_Init();
	BSP_PSENSOR_Init();
	BSP_MAGNETO_Init();
	max3010x_init();
	mlx90615_init();
	printf("Initialisation WiFi: %d\n", WIFI_Init());

#if NFC_ENABLE
	while (TT4_Init() != SUCCESS)
		;
#endif // NFC_ENABLE

	// Banner
	// // recuperer les données des capteurs environementaux
	// printf("\n\n****** HealthMonitor ******\n");
	// printf("BSP ver: %08lx\n", BSP_GetVersion());
	// printf("H-sensor ID: %u\n", BSP_HSENSOR_ReadID());
	// printf("P-sensor ID: %u\n", BSP_PSENSOR_ReadID());

	// // affichage des valeurs des capteurs
	//
	// printf("\n\n****** Capteur Result ******\n");
	// printf("Temperature: %f\n", BSP_TSENSOR_ReadTemp());
	// printf("Humidity: %f\n", BSP_HSENSOR_ReadHumidity());
	// printf("Pressure: %f\n", BSP_PSENSOR_ReadPressure());
	// mlx90615_init(); // intialisation capteur infrarouge
	// printf("IR sensor ID: %08lx\n", mlx90615_get_id()); // lecture ID du capteur infrarouge
	//
	// //calibration capteur
	printf("\n\n****** Calibration de l'accelerometre ******\n");
	HAL_Delay(3000);
	BSP_ACCELERO_AccGetXYZ(acceleroDataBase);
	sumAcceleroBase = acceleroDataBase[0] + acceleroDataBase[1]+ acceleroDataBase[2];
	printf("\n\n****** Calibration de l'accelerometre finie ******\n");

	// Network initialization

	// Wi-Fi connection

	if (WIFI_Connect(WLAN_SSID, WLAN_PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
	{
		WIFI_GetIP_Address(ip);
		printf("Connected to AP %s, IP address: %d.%d.%d.%d\n", WLAN_SSID,
			   ip[0], ip[1], ip[2], ip[3]);
		WIFI_GetMAC_Address(mac);
		printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1],
			   mac[2], mac[3], mac[4], mac[5]);
	}
	else
	{
		printf("Failed to connect to AP %s\n", WLAN_SSID);
	}

	// WIFI_OpenClientConnection(socket, WIFI_TCP_PROTOCOL, "TCP", RemoteIP,
	// 						  RemotePort, 0);
	// if (WIFI_SendLine(socket, "HELLO SERVER") == 0)
	// {
	// 	printf("Message TCP envoyé avec succès\n");
	// }
	// else
	// {
	// 	printf("Erreur lors de l'envoi du message TCP\n");
	// }
	// char string[256];
	// if (WIFI_ReadLine(socket, string) == 0)
	// {
	// 	printf("Received: %s\n", string);
	// }
	// else
	// {
	// 	printf("Erreur lors de la lecture du message TCP\n");
	// }

	// WIFI_CloseClientConnection(socket);
	
	// getDate();
	
#if NFC_ENABLE
	sURI_Info uri;
	char url[256];
	snprintf(url, sizeof(url), "192.168.0.254:8080/monitor?mac=%02X:%02X:%02X:%02X:%02X:%02X",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	strcpy(uri.protocol, URI_ID_0x03_STRING);
	strcpy(uri.URI_Message, url);
	uri.Information[0] = '\0';
	
	if (TT4_WriteURI(&uri) == SUCCESS) {
		printf("NFC tag configured successfully\n");
	} else {
		printf("Failed to configure NFC tag\n");
	}
#endif
	
	time_last_second = HAL_GetTick();
	//  Main loop
	while (1)
	{
		if(AppFlags.sensor_update) {
			uint32_t tick_start = HAL_GetTick();
			float temp = BSP_TSENSOR_ReadTemp();
			float humidity = BSP_HSENSOR_ReadHumidity();
			float pressure = BSP_PSENSOR_ReadPressure();
			time_sensor_update = HAL_GetTick() - tick_start;
			time_busy_total += time_sensor_update;
			AppFlags.sensor_update = 0;
		}
		
		if(AppFlags.data_tx){
			uint32_t tick_start = HAL_GetTick();
			postData(BSP_TSENSOR_ReadTemp(), BSP_HSENSOR_ReadHumidity(), BSP_PSENSOR_ReadPressure(), BSP_TSENSOR_ReadTemp(), BSP_HSENSOR_ReadHumidity(), BSP_PSENSOR_ReadPressure(), AppFlags.falling);
			time_network_tx = HAL_GetTick() - tick_start;
			time_busy_total += time_network_tx;
			
			uint32_t elapsed = HAL_GetTick() - time_last_second;
			float cpu_usage = (time_busy_total * 100.0f) / elapsed;
			
			printf("\n===== TEMPS D'EXECUTION =====\n");
			printf("Capteurs: %lu ms\n", time_sensor_update);
			printf("MAX3010x: %lu ms\n", time_max3010x_calc);
			printf("Reseau: %lu ms\n", time_network_tx);
			printf("Temps actif total: %lu ms sur %lu ms\n", time_busy_total, elapsed);
			printf("Occupation CPU: %.1f%%\n", cpu_usage);
			printf("Temps inactif: %.1f%%\n", 100.0f - cpu_usage);
			printf("==============================\n\n");
			
			time_busy_total = 0;
			time_last_second = HAL_GetTick();
			AppFlags.data_tx = 0;
		}
		detectFall();
	}

	// Error handling
	printf("SYSTEM ERROR");
	while (1)
	{
		// rien d'autre à faire
	}
}

void Error_Handler(void)
{
	printf("FATAL ERROR");
}

void max3010x_INT_Callback(uint8_t intStatus)
{

	if (intStatus & (1u << 6))
	{
		AppFlags.max3010x_data_ready = 1;
	}
}

void HAL_SYSTICK_Callback(void)
{
	for (unsigned int appTimerIndex = 0;
		 appTimerIndex < sizeof(AppTimers) / sizeof(struct _AppTimer_t);
		 appTimerIndex++)
	{
		if (HAL_GetTick() % AppTimers[appTimerIndex].delayInMs == 0)
		{
			*AppTimers[appTimerIndex].flagPtr = 1;
		}
	}
}

void USER_Button_Callback(void)
{
	AppFlags.user_button_pressed = 1;
}

int WIFI_ReadLine(uint8_t socket, char *string)
{
	static char buffer[256];
	static int buffer_pos = 0;
	static int buffer_size = 0;
	uint16_t received_bytes;
	int string_pos = 0;
	int line_found = 0;

	string[0] = '\0';

	while (!line_found)
	{
		if (buffer_pos >= buffer_size)
		{
			buffer_pos = 0;
			buffer_size = 0;

			if (WIFI_ReceiveData(socket, (uint8_t *)buffer, sizeof(buffer),
								 &received_bytes, 5000) != WIFI_STATUS_OK)
			{
				return -1;
			}

			if (received_bytes == 0)
			{
				return -1;
			}

			buffer_size = received_bytes;
		}

		while (buffer_pos < buffer_size && !line_found)
		{
			if (buffer[buffer_pos] == '\n' || buffer[buffer_pos] == '\r')
			{
				line_found = 1;
				buffer_pos++;

				if (buffer_pos < buffer_size && ((buffer[buffer_pos - 1] == '\r' && buffer[buffer_pos] == '\n') || (buffer[buffer_pos - 1] == '\n' && buffer[buffer_pos] == '\r')))
				{
					buffer_pos++;
				}
			}
			else
			{
				string[string_pos++] = buffer[buffer_pos++];

				if (string_pos >= 255)
				{
					string[string_pos] = '\0';
					return -1;
				}
			}
		}
	}

	string[string_pos] = '\0';
	return 0;
}

int WIFI_SendLine(uint8_t socket, const char *string)
{
	uint16_t bytesSent;
	char data[258];
	snprintf(data, sizeof(data), "%s\r\n", string);
	int length = strlen(data);

	if (length >= 256)
	{
		printf("Error: Invalid string length\n");
		return -1;
	}

	if (WIFI_SendData(socket, (uint8_t*)data, length, &bytesSent, 5000) != WIFI_STATUS_OK)
	{
		printf("Error: Failed to send data\n");
		return -1;
	}

	if (bytesSent != length)
	{
		printf("Error: Failed to send all data\n");
		return -1;
	}

	return 0;
}

int WIFI_SendHTTPRequest(uint8_t socket, const char *method, const char *path,
						 const char *host, const char *data)
{
	char buffer[512];

	snprintf(buffer, sizeof(buffer), "%s %s HTTP/1.0", method, path);
	WIFI_SendLine(socket, buffer);
	snprintf(buffer, sizeof(buffer), "Host: %s", host);
	WIFI_SendLine(socket, buffer);
	snprintf(buffer, sizeof(buffer), "User-Agent: B-L475EIOT01 %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	WIFI_SendLine(socket, buffer);
	WIFI_SendLine(socket, "Content-Type: application/x-www-form-urlencoded");
	snprintf(buffer, sizeof(buffer), "Content-Length: %d", strlen(data));
	WIFI_SendLine(socket, buffer);
	WIFI_SendLine(socket, "Connection: close");
	WIFI_SendLine(socket, "");
	if (data && strlen(data) > 0)
	{
		WIFI_SendLine(socket, data);
	}

	return 0;
}

int getDate(void)
{
	WIFI_OpenClientConnection(socket, WIFI_TCP_PROTOCOL, "TCP", RemoteIP,
							  RemotePort, 0);
	WIFI_SendHTTPRequest(socket, "GET", "/date", "192.168.0.254", "");
	char string[256];
	while (WIFI_ReadLine(socket, string) == 0)
	{
		printf("%s\n", string);
	}
	WIFI_CloseClientConnection(socket);
	return 0;
}

int postData(float btemp, float bhumidity, float bpressure, float temp, float pulse, float ox, int falling)
{
	WIFI_OpenClientConnection(socket, WIFI_TCP_PROTOCOL, "TCP", RemoteIP,
							  RemotePort, 0);

	char data[256];
	snprintf(data, sizeof(data), "btemp=%.2f&bhumidity=%.2f&bpressure=%.2f&temp=%.2f&pulse=%.2f&ox=%.2f&falling=%d",
			 btemp, bhumidity, bpressure, temp, pulse, ox, falling);

	if (WIFI_SendHTTPRequest(socket, "POST", "/monitor", "192.168.0.254", data) != 0)
	{
		printf("Error: Failed to send HTTP request\n");
		return -1;
	}
	char string[256];
	while (WIFI_ReadLine(socket, string) == 0)
	{
		printf("%s\n", string);
	}
	WIFI_CloseClientConnection(socket);
	return 0;
}

int detectFall(void)
{
	BSP_ACCELERO_AccGetXYZ(acceleroData);
	sumAccelero = acceleroData[0] + acceleroData[1] + acceleroData[2];
	if (abs(sumAccelero - sumAcceleroBase) > 200)
	{
		printf("\n\n****** Chute Detecter ******\n");
		AppFlags.falling = 1;
	}
	else
	{
		if (AppFlags.falling)
		{
			printf("\n\n****** Chute Terminer, en attente de confirmation ******\n");
		}
	}
	HAL_Delay(1000);

	// detection de chute

	if (AppFlags.max3010x_data_ready)
	{
		AppFlags.max3010x_data_ready = 0;
		uint32_t tick_start = HAL_GetTick();
		uint8_t count = max3010x_getUnreadSampleCount();
		SAMPLE samples[32];

		max3010x_getFIFO(samples, count);
		max3010x_cal();
		time_max3010x_calc = HAL_GetTick() - tick_start;
		time_busy_total += time_max3010x_calc;
		if (AppFlags.uart_print)
		{
			if (max3010x_isHeartRateValid())
			{
				printf("Heart Rate: %d bpm\n", max3010x_getHeartRate());
			}

			if (max3010x_isSpO2Valid())
			{
				printf("SpO2: %d %%\n", max3010x_getSpO2());
			}
			AppFlags.uart_print = 0;
		}
	}
	// gestion appui bouton
	if (AppFlags.user_button_pressed && AppFlags.falling)
	{
		AppFlags.user_button_pressed = 0;
		AppFlags.falling = 0;
		printf("[INFO] Visite effectuee. Alerte de chute réinitialisee.\n");
	}
	return 0;
}