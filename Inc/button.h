/*
 * BUTTON.H
 *
 *  Created on: May 28, 2024
 *      Author: ADMIN
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE SET
#define PRESSED_STATE RESET
#define LONG_PRESS_TIME 3000
#define LONG_PRESS_INTERVAL 1000
#define NO_EVENT_TIMEOUT 10000


void getKeyInput();
int is_button_pressed ( int index );
int is_button_long_pressed(int index);
int is_any_button_pressed();

#endif /* INC_BUTTON_H_ */
