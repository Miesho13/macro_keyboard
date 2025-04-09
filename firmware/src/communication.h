#ifndef __COMMUNICATION__
#define __COMMUNICATION__


void usb_handle();
void comm_send(); 

/*
 * This init is for incializaton
 * aplication
 */
void communication_init();
/*
 * This service is for handle communication
 * wiht num_kyepad and computer. This 
 * service shoud take KEY 
 */
void communication_service();

#endif
