#ifndef FT232R_SPI_H
#define FT232R_SPI_H


#ifdef _WIN32
#include <Windows.h>
#include <ftd2xx.h>
#endif


#ifdef __linux__
#include <ftdi.h>
#endif

class FT232R_SPI
{
public:
    FT232R_SPI();
    void ftdi_init_pins(unsigned char CSN, unsigned char SCK, unsigned char MOSI, unsigned char MISO, unsigned char CE);
    void ftdi_spi_rw(unsigned char *write, unsigned char *read, int size);

private:

#ifdef _WIN32
    FT_DEVICE_LIST_INFO_NODE * devlist;
    FT_HANDLE ftHandle;
#elif __linux__
    struct ftdi_context *ftdi;
#endif

    unsigned char CSN;
    unsigned char SCK;
    unsigned char MOSI;
    unsigned char MISO;
    unsigned char CE;

    unsigned char state;

    int ftdi_byte_encode(unsigned char *array, unsigned char byte);
    unsigned int ftdi_spi_decode(unsigned char *inResponse, unsigned int inResponseSize, unsigned char *outBuff);
};

#endif // FT232R_SPI_H
