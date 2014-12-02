#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QTimer>
#include <QStringListModel>
#include <QDateTime>
#include <QRegExp>
#include <QByteArray>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <ftd2xx.h>
#include "../../nrf24l01_lib/nrf24l01.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FT_DEVICE_LIST_INFO_NODE * devlist;
    FT_HANDLE ftHandle;

    void addByte(QByteArray * array, unsigned char byte);
    unsigned char state;
    void nrf24l01p_spi_rw(unsigned char * write, unsigned char * read, int size);
    unsigned int nrf24l01p_spi_decode(unsigned char *inResponse, unsigned int inResponseSize, unsigned char * outBuff);

    QString getMaskFromRX1();
    QString extractAddress(QString maskedAddress);
    QString convertAddress(unsigned char * addr);
	bool changeAddrMask(int width);

    uint8_t nrf24l01p_read(uint8_t reg, uint8_t * mem, int size);
    uint8_t nrf24l01p_write(uint8_t reg, uint8_t * mem, int size);
    uint8_t nrf24l01p_read_byte(uint8_t reg);
    void nrf24l01p_write_byte(uint8_t reg, uint8_t value);

	int randInt(int low, int high);

	void save();
    QTimer * timer;

	 int oldEN_AA;



public slots:
    void onOpen();
    void onSendCmd();
	void onTestConnection();
    void onAddressWidthChanged();
    void onRX1Changed();

    void onLNAGain(bool state);
    void onRFChannel(int value);
    void onRFOutPower(int value);
    void onSpeedChange();
    void onCRCChanged(bool state);
    void onCRCSizeChange();
    void onAutoRetransmit(bool state);
    void onRetransmitCount(int count);
    void onRetransmitDelay(int delay);

    void onPipeEnable(bool state);
    void onAddressChange();
    void onAckEnable(bool state);
    void onFixedSizeEnable(bool state);
    void onPipeSizeChange(int newSize);

	void onModeChange();
	void onTabModeChange(int index);

	void onSendPacket();

    void checkNewPacket();

	void configPinouts();
	
};

#endif // MAINWINDOW_H
