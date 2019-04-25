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

#define RMT_RX_CHANNEL RMT_CHANNEL_0


void rx_channels_init(rmt_channel_t rmt_rx_channel, int rx_gpio, int rx_mem_block_num);

static void rx_debug_task();

static void rmt_rx_receive_command(void* parameter);


commands receive_commands(char* brand, char* model, char* func);

int dur( uint32_t level, uint32_t duration);








