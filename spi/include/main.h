#include <stdio.h>
#include <string.h>

/*
* @brief 
  Set all the configuration for SPI Config structure like
  frequency, Transfer Mode, Word Size and chip select
  device binding for SPI_1
*/
void enable_spi(void);

/*
* @brief
  Set the device binding for GPIO and
  configure the GPIO pin
*/
void enable_gpio(void);

/*
* @brief Read/write the specified amount of data from the SPI driver.
  @param dev Pointer to the device structure for the driver instance
  @param config Pointer to a valid spi_config structure instance.
  @param Tx_SPI_data Buffer array where data to be sent originates from.
  @param Rx_SPI_data Buffer array where data to be read will be written to.
  @return 0 If transmission successful. 
*/
int spi_write_read(struct device *dev, struct spi_config *config, char *Tx_SPI_data, char *Rx_SPI_data);