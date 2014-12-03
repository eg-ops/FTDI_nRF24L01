#include "ft232r_spi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FT232R_SPI::FT232R_SPI()
{
}

void FT232R_SPI::ftdi_init_pins(unsigned char CSN, unsigned char SCK, unsigned char MOSI, unsigned char MISO, unsigned char CE)
{
    this->CSN = CSN;
    this->SCK = SCK;
    this->MOSI = MOSI;
    this->MISO = MISO;
    this->CE = CE;
}


void FT232R_SPI::ftdi_spi_rw(unsigned char *write, unsigned char *read, int size){

    unsigned char * buffWrite = (unsigned char *)malloc((size+2)*((8*3)+3));
    unsigned char * buffRead = (unsigned char *)malloc((size+2)*((8*3)+3));
    int index = 0;


    state &= ~CSN;
    buffWrite[index++] = state;

    for(int i = 0; i < size; i++){
        index += ftdi_byte_encode(buffWrite+index, write[i]);
    }

    state |= CSN;
    buffWrite[index++] = state;


#ifdef _WIN32
    DWORD written = 0;
    FT_Write(ftHandle,(void*)buffWrite, index, &written);
    written = 0;
    FT_Read(ftHandle, (void*) buffRead, index, &written);
#elif __linux__
    unsigned int written = 0;
    written = ftdi_write_data(ftdi, (unsigned char*)buffWrite, index);
    ftdi_read_data(ftdi, (unsigned char*) buffRead, index);
#endif

    ftdi_spi_decode(buffRead, written, read);

    free(buffWrite);
    free(buffRead);

}

unsigned int FT232R_SPI::ftdi_spi_decode(unsigned char * inResponse, unsigned int inResponseSize, unsigned char * outBuff){
    int state = -1;
    int count = 0;
    int index = 0;
    int tmp = 0;
    for (int i = 0; i < (int)inResponseSize; i++){
        unsigned char b = inResponse[i];
        if (state != (b & SCK)){
            if (b & SCK){
                if (b & MISO){
                    tmp |= (1 << (7 - count));
                }
                if (count < 7){
                    count++;
                } else {
                    outBuff[index++] = tmp;
                    count = 0;
                    tmp = 0;
                }
            }
            state = (b & SCK);
        }
    }
    return index;
}


int FT232R_SPI::ftdi_byte_encode(unsigned char * array, unsigned char byte){

    int index = 0;

    array[index++] = state;

    for(int i = 0; i < 8; i++){
        bool isHIGH = (1 << (7-i)) & byte;
        bool isLast = i == 7;
        bool isNextHIGH = (!isLast &&  ((1 << (7-i-1)) & byte));
        if (isHIGH) {
            state |= MOSI;
        } else {
            state &= ~MOSI;
        }
        array[index++] = state;
        state |= SCK;
        array[index++] = state;
        if (!isNextHIGH || isLast){
            state &= ~MOSI;
        }
        state &= ~SCK;
        array[index++] = state;

    }

    array[index++] = state;
    return index;
}
