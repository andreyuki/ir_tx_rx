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
#include "rmt_structs.h"

// FreeRTOS function
#define INCLUDE_vTaskDelay 1

#define rrmt_item32_tIMEOUT_US  9500   /*!< RMT receiver timeout value */
#define RMT_TX_CHANNEL RMT_CHANNEL_4

void tx_channels_init(rmt_channel_t rmt_tx_channel, int tx_gpio, int tx_mem_block_num);

static void rmt_tx_task(void* parameter);

esp_err_t send_commands(rmt_items* items);

int dur( uint32_t level, uint32_t duration);















