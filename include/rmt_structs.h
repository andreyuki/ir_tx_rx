/*
 * rmt_structs.h
 *
 *  Created on: 18 de abr de 2019
 *      Author: dev
 */

#ifndef COMPONENTS_IR_TX_RX_INCLUDE_RMT_STRUCTS_H_
#define COMPONENTS_IR_TX_RX_INCLUDE_RMT_STRUCTS_H_

#include "driver/rmt.h"

#define COMMAND_STRUCT_STRING_LENGTH 15

typedef struct{
	int number_of_items;
	rmt_item32_t items[128];
}rmt_items;

typedef struct{
	char brand[COMMAND_STRUCT_STRING_LENGTH];
	char model[COMMAND_STRUCT_STRING_LENGTH];
	char func[COMMAND_STRUCT_STRING_LENGTH];
	rmt_items rmt;
}commands;


#endif /* COMPONENTS_IR_TX_RX_INCLUDE_RMT_STRUCTS_H_ */
