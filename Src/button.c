#include "button.h"

#define TIME_FOR_PRESS_KEY 10 // assuming this is 10ms for debounce
#define NUM_BUTTONS 3
#define NO_EVENT_TIMEOUT 10000 // 10 seconds
#define LONG_PRESS_TIME 3000 // 3 seconds
#define LONG_PRESS_INTERVAL 1000 // 1 second

int KeyReg0[NUM_BUTTONS] = { SET };
int KeyReg1[NUM_BUTTONS] = { SET };
int KeyReg2[NUM_BUTTONS] = { SET };
int KeyReg3[NUM_BUTTONS] = { SET };

int TimeOutForKeyPress = TIME_FOR_PRESS_KEY;
int button_flag[NUM_BUTTONS] = { RESET };
int button_long_press_flag[NUM_BUTTONS] = { RESET };
int button_press_duration[NUM_BUTTONS] = { 0 };
int long_press_interval_timer[NUM_BUTTONS] = { 0 };
int no_event_timer = NO_EVENT_TIMEOUT;  // Initialize the no_event_timer

int is_button_pressed(int index) {
    if (button_flag[index] == 1) {
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

int is_button_long_pressed(int index) {
    if (button_long_press_flag[index] == 1) {
        button_long_press_flag[index] = 0;
        return 1;
    }
    return 0;
}

int is_any_button_pressed() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (KeyReg0[i] == PRESSED_STATE) {
            return 1;
        }
    }
    return 0;
}

void subKeyProcess(int index) {
    button_flag[index] = 1;
}

void subLongKeyProcess(int index) {
    button_long_press_flag[index] = 1;
}

void getKeyInput() {
    int anyButtonPressed = 0;

    for (int i = 0; i < NUM_BUTTONS; i++) {
        KeyReg2[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg0[i];
        switch (i) {
            case 0:
                KeyReg0[i] = HAL_GPIO_ReadPin(RESET_GPIO_Port, RESET_Pin);
                break;
            case 1:
                KeyReg0[i] = HAL_GPIO_ReadPin(INCR_GPIO_Port, INCR_Pin);
                break;
            case 2:
                KeyReg0[i] = HAL_GPIO_ReadPin(DECR_GPIO_Port, DECR_Pin);
                break;
            default:
                break;
        }

        if ((KeyReg1[i] == KeyReg0[i]) && (KeyReg1[i] == KeyReg2[i])) {
            if (KeyReg2[i] != KeyReg3[i]) {
                KeyReg3[i] = KeyReg2[i];

                if (KeyReg3[i] == PRESSED_STATE) {
                    subKeyProcess(i);
                    button_press_duration[i] = 0;
                    long_press_interval_timer[i] = LONG_PRESS_INTERVAL;
                    anyButtonPressed = 1;
                }
            } else {
                if (KeyReg2[i] == PRESSED_STATE) {
                    button_press_duration[i] += TIME_FOR_PRESS_KEY;

                    if (button_press_duration[i] >= LONG_PRESS_TIME) {
                        if (long_press_interval_timer[i] <= 0) {
                            subLongKeyProcess(i);
                            long_press_interval_timer[i] = LONG_PRESS_INTERVAL;
                        } else {
                            long_press_interval_timer[i] -= TIME_FOR_PRESS_KEY;
                        }
                    }
                    anyButtonPressed = 1;
                }
                TimeOutForKeyPress--;
                if (TimeOutForKeyPress == 0) {
                    TimeOutForKeyPress = TIME_FOR_PRESS_KEY;
                }
            }
        } else {
            button_press_duration[i] = 0;
            long_press_interval_timer[i] = LONG_PRESS_INTERVAL;
        }
    }

    if (anyButtonPressed) {
        no_event_timer = NO_EVENT_TIMEOUT;
    } else {
        no_event_timer -= TIME_FOR_PRESS_KEY;
    }
}
