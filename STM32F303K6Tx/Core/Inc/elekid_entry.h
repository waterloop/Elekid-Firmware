#ifndef INC_ELEKID_ENTRY_H_
#define INC_ELEKID_ENTRY_H_

extern uint16_t ADCValues[64];
float VSense[2]; 	// first one is 5V, second one is 24V
float ISense[2]; 	// first one is 5V, second one is 24V
extern float offset[4];

int psyduck_entry();

#endif /* INC_ELEKID_ENTRY_H_ */
