deps_config := \
	/home/dev/esp/esp-idf/components/app_trace/Kconfig \
	/home/dev/esp/esp-idf/components/aws_iot/Kconfig \
	/home/dev/esp/esp-idf/components/bt/Kconfig \
	/home/dev/esp/esp-idf/components/driver/Kconfig \
	/home/dev/esp/esp-idf/components/esp32/Kconfig \
	/home/dev/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/dev/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/dev/esp/esp-idf/components/ethernet/Kconfig \
	/home/dev/esp/esp-idf/components/fatfs/Kconfig \
	/home/dev/esp/esp-idf/components/freertos/Kconfig \
	/home/dev/esp/esp-idf/components/heap/Kconfig \
	/home/dev/esp/esp-idf/components/libsodium/Kconfig \
	/home/dev/esp/esp-idf/components/log/Kconfig \
	/home/dev/esp/esp-idf/components/lwip/Kconfig \
	/home/dev/esp/esp-idf/components/mbedtls/Kconfig \
	/home/dev/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/dev/esp/esp-idf/components/openssl/Kconfig \
	/home/dev/esp/esp-idf/components/pthread/Kconfig \
	/home/dev/esp/esp-idf/components/spi_flash/Kconfig \
	/home/dev/esp/esp-idf/components/spiffs/Kconfig \
	/home/dev/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/dev/esp/esp-idf/components/vfs/Kconfig \
	/home/dev/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/dev/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/dev/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/dev/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/dev/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
