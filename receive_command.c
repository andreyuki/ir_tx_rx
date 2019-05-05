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
#include "receive_command.h"
#include "rmt_structs.h"
#include <unistd.h>

static const char *TAG = "receive_com_events";

EventGroupHandle_t xEventGroup;

// Inicialização do canal de RX
void rx_channels_init(rmt_channel_t rmt_rx_channel, int rx_gpio, int rx_mem_block_num) {

	rmt_config_t rmt_rx_config;

	rmt_rx_config.rmt_mode      = RMT_MODE_RX;
	rmt_rx_config.channel       = rmt_rx_channel;
	rmt_rx_config.gpio_num      = rx_gpio;
	rmt_rx_config.clk_div       = 80;
	rmt_rx_config.mem_block_num = rx_mem_block_num;

	rmt_rx_config.rx_config.filter_en           = false;
	rmt_rx_config.rx_config.filter_ticks_thresh = 0;
	rmt_rx_config.rx_config.idle_threshold      = 50000;

	rmt_config(&rmt_rx_config);
	rmt_driver_install(rmt_rx_config.channel, 1024, 0);
}


// Task RX para recebimento contínuo
static void rx_debug_task() {
	size_t i;
    size_t rx_size = 0;
    rmt_item32_t* items = NULL;

    // define ringbuffer handle
    RingbufHandle_t rx_rb;

    // start receiving IR data

	rmt_rx_start(RMT_RX_CHANNEL, 1);

    // loop forever
	while (1) {
		// get the ring buffer handle
		rmt_get_ringbuf_handle(RMT_RX_CHANNEL, &rx_rb);

		// get items, if there are any
		items = (rmt_item32_t*) xRingbufferReceive(rx_rb, &rx_size, 10);
		if(items) {

			// print the RMT received durations to the monitor
			ESP_LOGI(TAG, "Received %i items\n", rx_size/4 );
			for ( i=0; i<rx_size/4; i++ ) {
				ESP_LOGI(TAG, "%d: %i, %i", i, dur(items[i].level0, items[i].duration0), dur(items[i].level1, items[i].duration1) );
			}

			// free up data space
			vRingbufferReturnItem(rx_rb, (void*) items);
		}
	// delay 100 milliseconds
	vTaskDelay( 100 / portTICK_PERIOD_MS );
	}
}

//Task RX para recebimento do comando
static void rmt_rx_receive_command(void* parameter) {
	size_t i;
    size_t rx_size = 0;
    rmt_item32_t* items = NULL;
    rmt_item32_t* items_to_record = NULL;

    commands *command = (commands*)parameter;

    // define ringbuffer handle
    RingbufHandle_t rb;

    // start receiving IR data
	rmt_rx_start(RMT_RX_CHANNEL, 1);

	while (1) {
		// get the ring buffer handle
		rmt_get_ringbuf_handle(RMT_RX_CHANNEL, &rb);

		// get items, if there are any
		items = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 10);

		if(items) {
			// print the RMT received durations to the monitor
			items_to_record = malloc(rx_size);
			for ( i=0; i<rx_size/4; i++ ) {
				items_to_record[i].level0 = !items[i].level0;
				items_to_record[i].duration0 = items[i].duration0;
				items_to_record[i].level1 = !items[i].level1;
				items_to_record[i].duration1 = items[i].duration1;
			}

			ESP_LOGI(TAG, "Received %i items\n", rx_size/4 );
			for ( i=0; i<rx_size/4; i++ ) {
				ESP_LOGI(TAG, "%d: %i, %i", i, dur(items[i].level0, items[i].duration0), dur(items[i].level1, items[i].duration1) );
			}

			memcpy (command->rmt.items, items_to_record, rx_size);
			command->rmt.number_of_items = rx_size/4;

			// free up data space
			vRingbufferReturnItem(rb, (void*) items);
			xEventGroupSetBits(xEventGroup, BIT0);
			//xTaskCreate(rx_debug_task, "rx_debug_task", 2048, NULL, 10, NULL);
			break;
		}
		vTaskDelay( 100 / portTICK_PERIOD_MS );
	}

    vTaskDelete(NULL);
}

//Função para receber comando, recebe como parâmetro informações do comando e retorna struct commands com as informações, items e quantidade de itens
commands receive_commands(char* brand, char* model, char* func) {
	commands command;

	xEventGroup = xEventGroupCreate();

	snprintf(command.brand, COMMAND_STRUCT_STRING_LENGTH, "%s", brand);
	snprintf(command.model, COMMAND_STRUCT_STRING_LENGTH, "%s",  model);
	snprintf(command.func, COMMAND_STRUCT_STRING_LENGTH, "%s", func);

	xTaskCreate(rmt_rx_receive_command, "rmt_rx_receive_command", 2048, (void*)&command, 10, NULL);

	xEventGroupWaitBits(xEventGroup, BIT0, pdFALSE, pdFALSE, portMAX_DELAY);

	ESP_LOGI(TAG,"BRAND: %s", command.brand);
	ESP_LOGI(TAG,"MODEL: %s", command.model);
	ESP_LOGI(TAG,"FUNC: %s", command.func);

	ESP_LOGI(TAG, "Recorded %i items\n", command.rmt.number_of_items );
	for (int i=0; i<command.rmt.number_of_items; i++ ) {
		ESP_LOGI(TAG, "%d: %i %i", i, dur(command.rmt.items[i].level0, command.rmt.items[i].duration0), dur(command.rmt.items[i].level1, command.rmt.items[i].duration1 ));
	}

	return command;

}

/*
 *   Função que transforma o nível do RMT em positivo e negativo para melhor visualização
 */
int dur( uint32_t level, uint32_t duration ) {
	if ( level == 0 ) { return duration; }
	else { return -1.0 * duration; }
}






