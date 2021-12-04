#include "threads.h"

const osThreadAttr_t can_attributes = {
  .name = "can",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

void can_th_fn(void *argument)
{
  for(;;)
  {
      osMutexAcquire(sensorVal_mtx, osWaitForever);
      CANFrame tx_frame0 = CANFrame_init(CURRENT_5V_DATA);
      CANFrame_set_field(&tx_frame0, CURRENT_5V, FLOAT_TO_UINT(ISense[0]));

      CANFrame tx_frame1 = CANFrame_init(CURRENT_24V_DATA);
      CANFrame_set_field(&tx_frame1, CURRENT_24V, FLOAT_TO_UINT(ISense[1]));
      osMutexRelease(sensorVal_mtx);

      osDelay(HEARTBEAT_PERIODICITY);
  }
}
