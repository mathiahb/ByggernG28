#ifndef USER_INTERFACE_GUARD
#define USER_INTERFACE_GUARD

typedef enum{
    START_MENU = 0,
    TEMPLATE = 999
}MENU_ID;

void init_user_interface();
void update_user_interface();
void bind_callback(void (*callback_function)(MENU_ID, int));

#endif