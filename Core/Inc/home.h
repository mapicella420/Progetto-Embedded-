/*
 * home.h
 *
 *  Created on: May 22, 2024
 *      Author: mapic
 */

#ifndef INC_HOME_H_
#define INC_HOME_H_

typedef struct {
    float x[200];
    float y[200];
    float HT;
    float HT_variability;


} Data;


void init_home();

#endif /* INC_HOME_H_ */
