#include "threads.h"

const osThreadAttr_t measurement_attributes = {
  .name = "measurement",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void measurement_th_fn(void *argument)
{
    HAL_ADC_Start(&hadc2);

    for(;;)
    {
        osMutexAcquire(ADCVal_mtx, osWaitForever);
            //read 16 values from each sensor at a time for precision
            for (size_t i = 0; i < 64; ++i)
            {
                //might have to turn off dma mode
                if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
                {
                    ADCValues[i] = HAL_ADC_GetValue(&hadc2);
                }
                else
                {
                    //osMutexRelease(ADCVal_mtx);
                    Error_Handler();
                }
            }
        osMutexRelease(ADCVal_mtx);

        //give up the processor
        osThreadYield();
    }
}
