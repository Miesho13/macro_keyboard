#ifndef __UART__
#define __UART__

#include "type.h"

void uart_init(void);
int  uart_read(u8 *output, u32 output_buffer);
void uart_write(u8 *input, u32 input_buffer);

#endif // UART
