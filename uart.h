

void init_uart();
void send_uart(char letter);
void guarantee_send_uart(char letter);
char uart_receive();
int uart_ready_to_send();
int uart_unread_data_in_buffer();