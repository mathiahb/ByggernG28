

void init_uart();
void send_uart(char letter);
char receive_uart();
int uart_ready_to_send();
int uart_unread_data_in_buffer();