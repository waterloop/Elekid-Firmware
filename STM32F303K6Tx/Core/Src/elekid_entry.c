#include <stdbool.h>
#include "cmsis_os.h"

#include "elekid_entry.h"
#include "can.h"
#include "main.h"
#include "threads.h"

uint16_t ADCValues[64] = {4096};
float VSense[2];
float ISense[2];
float offset[4] = {0, 0, 0, 0};

osThreadId_t measurement_th;
osThreadId_t processing_th;
osThreadId_t can_th;

osMutexId_t ADCVal_mtx;
osMutexId_t sensorVal_mtx;

int psyduck_entry()
{
    if (CANBus_init(&hcan) != HAL_OK) { Error_Handler(); }

    osKernelInitialize();

    measurement_th = osThreadNew(measurement_th_fn, NULL, &measurement_attributes);
    processing_th = osThreadNew(processing_th_fn, NULL, &processing_attributes);
    can_th = osThreadNew(can_th_fn, NULL, &can_attributes);

    if (measurement_th == NULL ||
        processing_th == NULL ||
        can_th == NULL)
    {
        Error_Handler();
    }

    ADCVal_mtx = osMutexNew(NULL);    //NULL for default attribs
    sensorVal_mtx = osMutexNew(NULL); //NULL for default attribs

    if (ADCVal_mtx == NULL ||
        sensorVal_mtx == NULL)
    {
        Error_Handler();
    }

    osKernelStart();

    Error_Handler();

    return 0;
}

