#ifndef __MLX90615_H_
#define __MLX90615_H_

void mlx90615_init(void);
uint32_t mlx90615_get_id(void);
float mlx90615_read_ambient_temp(void);
float mlx90615_read_temp(void);

#endif /*__MLX90615_H_ */
