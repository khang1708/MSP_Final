#include "fsm_countSystem.h"

extern int no_event_timer;  // Declare the no_event_timer as external

void wrapAroundCounter(int *n) {
    if (*n > 9) *n = 0;
    if (*n < 0) *n = 9;
}

int n = 0;

void fsm_countSystem() {
    if (no_event_timer <= 0) {
        if (n > 0) {
            n--;
        }
        no_event_timer = 1000;  // 1 second decrement interval
    }

    if (is_button_pressed(0) == 1) {
        n = 0;
        no_event_timer = NO_EVENT_TIMEOUT;
    }

    if (is_button_pressed(1) == 1) {
        n++;
        wrapAroundCounter(&n);
        no_event_timer = NO_EVENT_TIMEOUT;
    }

    if (is_button_pressed(2) == 1) {
        n--;
        wrapAroundCounter(&n);
        no_event_timer = NO_EVENT_TIMEOUT;
    }



    if (timer0_flag == 1) {
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        setTimer0(100);
    }

    if (timer1_flag == 1) {
        display7SEG(n);
    }
}
