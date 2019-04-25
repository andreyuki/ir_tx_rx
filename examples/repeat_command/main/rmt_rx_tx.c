#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"
#include "freertos/event_groups.h"
#include "send_command.h"
#include "receive_command.h"
#include "rmt_structs.h"


void app_main() {
	// initialize hardware
    rx_channels_init(RMT_CHANNEL_0, 21, 4);
    tx_channels_init(RMT_CHANNEL_4, 15, 4);

    commands command_TX;

    command_TX = receive_commands("marcaTeste", "modeloTeste", "funcTeste");
    while(1){
    	send_commands(&command_TX.rmt);
    }

}
