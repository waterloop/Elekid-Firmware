#include "threads.h"

#define ADC_VOLTAGE_CONVERSION 0.0008058608F
#define RSHUNT 0.5F
#define V2LP 62.5F
#define V2HP 580.151F
#define VGain_1 ADC_VOLTAGE_CONVERSION*V2LP*3/2
#define VGain_2 ADC_VOLTAGE_CONVERSION*V2HP
#define IGain ADC_VOLTAGE_CONVERSION/RSHUNT/50.0

const osThreadAttr_t processing_attributes = {
  .name = "processing",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void processing_th_fn(void *argument)
{
    //in the condition that ADCValues aren't populated yet
    //maybe don't need
    bool valueReady = false;
    do
    {
        osMutexAcquire(ADCVal_mtx, osWaitForever);
            if (ADCValues[0] == 4096)
                valueReady = true;
        osMutexRelease(ADCVal_mtx);

        osThreadYield();
    } while (valueReady == false);

    for(;;)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            float tmp = 0;

            osMutexAcquire(ADCVal_mtx, osWaitForever);
            for (size_t j = 0; j < 16; ++j)
            {
                tmp += ADCValues[i + 4*j];
            }
            osMutexRelease(ADCVal_mtx);

            osMutexAcquire(sensorVal_mtx, osWaitForever);
            switch (i)
            {
            case 0:
            	VSense[0] = (VGain_2*tmp/16) + offset[i];
                break;
            case 1:
            	VSense[1] = (VGain_1*tmp/16) + offset[i];
                break;
            case 2:
            	ISense[0] = (IGain*tmp/16) + offset[i];
                break;
            case 3:
            	ISense[1] = (IGain)*tmp/16 + offset[i];
                break;
            default:
                break;
            }
            osMutexRelease(sensorVal_mtx);

        }
        osThreadYield();
    }
}

