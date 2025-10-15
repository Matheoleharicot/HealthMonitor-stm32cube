#ifndef _DATA_TYPES
#define _DATA_TYPES

typedef enum _DataType_e
{
  NONE = 0,
  COUNTER,
  BOARD_TEMP,
  BOARD_HUMIDITY,
  BOARD_PRESSURE,
  SKIN_TEMP,
  PULSE,
  OX,
  FALL_STATE
} DataType_t;

#endif /* _DATA_TYPES */
