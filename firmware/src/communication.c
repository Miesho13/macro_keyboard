#include "communication.h"

void communication_init() { 

}

void communication_service() {
    usb_handle();
    comm_send(); 
}
