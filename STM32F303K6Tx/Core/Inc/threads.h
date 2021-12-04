#ifndef INC_THREADS_H_
#define INC_THREADS_H_


#include "cmsis_os.h"
#include "elekid_entry.h"
#include "can.h"
#include "main.h"

#define HEARTBEAT_PERIODICITY 200 //ms

extern osThreadId_t measurement_th;
extern const osThreadAttr_t measurement_attributes;
void measurement_th_fn(void *argument);

extern osThreadId_t processing_th;
extern const osThreadAttr_t processing_attributes;
void processing_th_fn(void *argument);

extern osThreadId_t can_th;
extern const osThreadAttr_t can_attributes;
void can_th_fn(void *argument);

extern osMutexId_t ADCVal_mtx;
extern osMutexId_t sensorVal_mtx;

#endif /* INC_THREADS_H_ */
