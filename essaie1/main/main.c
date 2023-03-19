#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define GPIO_MISO GPIO_NUM_19
#define GPIO_MOSI GPIO_NUM_23
#define GPIO_SCLK  GPIO_NUM_18
#define GPIO_CS   GPIO_NUM_5

#define RCV_HOST    SPI3_HOST

spi_device_handle_t spi;
esp_err_t ret;

void spiMasterInit(void)
{

    spi_bus_config_t buscfg = {0};
    buscfg.mosi_io_num = GPIO_MOSI;
    buscfg.miso_io_num = GPIO_MISO;
    buscfg.sclk_io_num = GPIO_SCLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    spi_device_interface_config_t devcfg = {
         .clock_speed_hz = 10 * 1000 * 1000,     // Clock out at 10 MHz
         .mode = 0,                              // SPI mode 0
         .spics_io_num = GPIO_CS,                // CS pin
         .queue_size = 1,
     };
     spi_bus_initialize(HSPI_HOST, &buscfg, 1);  // HSPI_HOST as master
     spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
}

void app_main(void)
{
    spiMasterInit();

    uint8_t n = 255;


    while(1)
    {

    	  esp_err_t ret;
    	    spiMasterInit();
    	    uint8_t n = 255;
    	    char sendbuf[16] = {0};
    	    char recvbuf[16] = {0};
    	    while (1) {
    	        spi_transaction_t t = {
    	            .length = 8 * 16,
    	            .tx_buffer = sendbuf,
    	            .rx_buffer = recvbuf,
    	        };
    	        //snprintf(sendbuf, sizeof(sendbuf), "Hello from master ESP-WROOM-32: %d", n);
    	        ret = spi_device_transmit(spi, &t);
    	        if (ret != ESP_OK) {
    	            ESP_LOGE("SPI_MASTER", "Failed to transmit: %s", esp_err_to_name(ret));
    	        } else {
    	            ESP_LOGI("SPI_MASTER", "Transmission successful: sent: %s, received: %s", sendbuf, recvbuf);
    	        }
    	        vTaskDelay(2000 / portTICK_PERIOD_MS);
    	        n--;
    	    }
    }
}

