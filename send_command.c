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
#include "rmt_structs.h"

static const char *TAG = "sent_com_events";

SemaphoreHandle_t xSemaphoreTX = NULL;

// Inicialização do canal de TX
void tx_channels_init(rmt_channel_t rmt_tx_channel, int tx_gpio, int tx_mem_block_num) {
	rmt_config_t rmt_tx_config;

	//TODO: receber como parametro GPIO, MEM_BLOCK_NUM, RESOLUCAO para calcular o clk_div
//	int clk_div;
//	rmt_source_clk_t* src_clk = NULL;
//
//	rmt_get_source_clk(rmt_tx_channel, src_clk);
//
//	clk_div = (int)src_clk / res;

	rmt_tx_config.rmt_mode = RMT_MODE_TX;
	rmt_tx_config.channel = rmt_tx_channel;
	rmt_tx_config.gpio_num = tx_gpio;
	rmt_tx_config.mem_block_num = tx_mem_block_num;
	rmt_tx_config.tx_config.loop_en = 0;
	rmt_tx_config.tx_config.carrier_en = 1;
	rmt_tx_config.tx_config.carrier_level = 1;
	rmt_tx_config.tx_config.carrier_duty_percent = 50;
    rmt_tx_config.tx_config.carrier_freq_hz = 38000;
	rmt_tx_config.tx_config.idle_output_en = 1;
	rmt_tx_config.tx_config.idle_level = 0;
	rmt_tx_config.clk_div = 80;

	ESP_ERROR_CHECK(rmt_config(&rmt_tx_config));
	ESP_ERROR_CHECK(rmt_driver_install(rmt_tx_config.channel, 0, 0));

	vSemaphoreCreateBinary(xSemaphoreTX);
}

//Task TX para envio do comando
static void rmt_tx_task(void* parameter) {

	xSemaphoreTake(xSemaphoreTX, portMAX_DELAY);

	size_t i;

	rmt_items* items_to_send = (rmt_items*) parameter;

	ESP_LOGI(TAG, "Tx buffer has %i items\n", items_to_send->number_of_items );
	for ( i=0; i<items_to_send->number_of_items; i++ ) {
		ESP_LOGI(TAG, "%d: %i, %i", i, dur(items_to_send->items[i].level0, items_to_send->items[i].duration0), dur(items_to_send->items[i].level1, items_to_send->items[i].duration1));
	}


	rmt_write_items(RMT_TX_CHANNEL, items_to_send->items, items_to_send->number_of_items, 1);
	ESP_LOGI(TAG, "Signal sent %i items\n", items_to_send->number_of_items );


	xSemaphoreGive(xSemaphoreTX);
	vTaskDelete(NULL);
}

//Função para envia comando, recebe como parâmetro, struct contendo os items e a quantidade de items.
esp_err_t send_commands(rmt_items* items) {
	esp_err_t ret = ESP_OK;

	if(xTaskCreate(rmt_tx_task, "rmt_tx_task", 2048, (void *) items, 10, NULL) != pdPASS){
		ret = ESP_ERR_NO_MEM;
	}

	return ret;
}






















