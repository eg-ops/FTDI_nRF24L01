#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardItem>
#include <QSettings>
#include "ftdipinout.h"
#include "hexvalidator.h"

//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qpng)
//Q_IMPORT_PLUGIN( qmng );

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	oldEN_AA = 0x3F;

	CSN = (1 << 0);
	SCK = (1 << 1);
	MOSI = (1 << 2);
	MISO = (1 << 3);
	CE = (1 << 4);

#ifdef __linux__
	ftdi = NULL;
#endif
    

    connect(ui->actionConnect, SIGNAL(triggered(bool)), this, SLOT(onOpen(bool)));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(scanDevices()));
    connect(ui->actionTestConnection, SIGNAL(triggered()), this, SLOT(onTestConnection()));
    connect(ui->actionPinout, SIGNAL(triggered()), this, SLOT(configPinouts()));
    connect(ui->addressWidth3, SIGNAL(clicked()), this ,SLOT(onAddressWidthChanged()));
    connect(ui->addressWidth4, SIGNAL(clicked()), this ,SLOT(onAddressWidthChanged()));
    connect(ui->addressWidth5, SIGNAL(clicked()), this ,SLOT(onAddressWidthChanged()));
    connect(ui->rx1Address, SIGNAL(textChanged(QString)), this, SLOT(onRX1Changed()));

    ui->mainToolBar->insertWidget(ui->actionRefresh, ui->devicesComboBox);


    connect(ui->lnaGain, SIGNAL(clicked(bool)), this, SLOT(onLNAGain(bool)));
    connect(ui->rfChannelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRFChannel(int)));
    connect(ui->rfOutPower, SIGNAL(valueChanged(int)), this, SLOT(onRFOutPower(int)));
    connect(ui->speed1Mbps, SIGNAL(clicked()), this, SLOT(onSpeedChange()));
    connect(ui->speed2Mbps, SIGNAL(clicked()), this, SLOT(onSpeedChange()));
    connect(ui->crcGroupBox, SIGNAL(clicked(bool)), this, SLOT(onCRCChanged(bool)));
    connect(ui->crc1Byte, SIGNAL(clicked()), this, SLOT(onCRCSizeChange()));
    connect(ui->crc2Bytes, SIGNAL(clicked()), this, SLOT(onCRCSizeChange()));
    connect(ui->autoRetransmitGroupBox, SIGNAL(clicked(bool)), this, SLOT(onAutoRetransmit(bool)));
    connect(ui->retransmitCount, SIGNAL(valueChanged(int)), this, SLOT(onRetransmitCount(int)));
    connect(ui->retransmitDelay, SIGNAL(valueChanged(int)), this, SLOT(onRetransmitDelay(int)));
    connect(ui->dynPayloadLen, SIGNAL(clicked(bool)), this, SLOT(onDynPayloadLen(bool)));

    connect(ui->enRX0, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));
    connect(ui->enRX1, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));
    connect(ui->enRX2, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));
    connect(ui->enRX3, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));
    connect(ui->enRX4, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));
    connect(ui->enRX5, SIGNAL(clicked(bool)), this, SLOT(onPipeEnable(bool)));

    connect(ui->autoAckRx0, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));
    connect(ui->autoAckRx1, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));
    connect(ui->autoAckRx2, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));
    connect(ui->autoAckRx3, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));
    connect(ui->autoAckRx4, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));
    connect(ui->autoAckRx5, SIGNAL(clicked(bool)), this, SLOT(onAckEnable(bool)));

    connect(ui->fixedPacketSizeRx0, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));
    connect(ui->fixedPacketSizeRx1, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));
    connect(ui->fixedPacketSizeRx2, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));
    connect(ui->fixedPacketSizeRx3, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));
    connect(ui->fixedPacketSizeRx4, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));
    connect(ui->fixedPacketSizeRx5, SIGNAL(clicked(bool)), this, SLOT(onFixedSizeEnable(bool)));


    connect(ui->packetSizeRx0, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));
    connect(ui->packetSizeRx1, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));
    connect(ui->packetSizeRx2, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));
    connect(ui->packetSizeRx3, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));
    connect(ui->packetSizeRx4, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));
    connect(ui->packetSizeRx5, SIGNAL(valueChanged(int)), this, SLOT(onPipeSizeChange(int)));

    connect(ui->txAddress, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx0Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx1Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx2Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx3Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx4Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));
    connect(ui->rx5Address, SIGNAL(editingFinished()), this, SLOT(onAddressChange()));

    QFontMetrics fm(ui->txAddress->font());
    int max_width = fm.width(QString("00:00:00:00:00")) * 1.15;

    ui->txAddress->setMaximumWidth(max_width);
    ui->rx0Address->setMaximumWidth(max_width);
    ui->rx1Address->setMaximumWidth(max_width);


    connect(ui->hexPayload, SIGNAL(clicked(bool)), this, SLOT(onHexChanged(bool)));
    connect(ui->sendPacketButoon, SIGNAL(clicked()), this, SLOT(onSendPacket()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabModeChange(int)));

    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setShowGrid(false);

	ui->tableWidget_2->setColumnWidth(0,200);
	ui->tableWidget_2->verticalHeader()->hide();
	ui->tableWidget_2->setShowGrid(false);


	timer = new QTimer(this);
	timer->setInterval(10);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkNewPacket()));


    setUiEnabled(false);


    scanDevices();

}

MainWindow::~MainWindow()
{
#ifdef __linux__
    ftdi_free(ftdi);
#endif
    delete ui;
}



void MainWindow::onOpen(bool opened){
    qDebug() << "onOpen" << opened;

    if (opened){

#ifdef _WIN32
    int index = ui->devicesComboBox->itemData(ui->devicesComboBox->currentIndex()).value<int>();

    if (FT_Open(index, &ftHandle) == FT_OK){
        qDebug() << "opened";
        FT_SetUSBParameters(ftHandle, 0, 0);
        FT_SetLatencyTimer(ftHandle, 2);
        FT_SetDivisor(ftHandle, 0);
        //FT_SetBaudRate(ftHandle, 9600);
        FT_SetBitMode(ftHandle, CSN | SCK | MOSI | CE, FT_BITMODE_SYNC_BITBANG );
        state = CSN | CE;
        DWORD written = 0;
        FT_Write(ftHandle,(void*)&state,sizeof(state), &written);
        qDebug() << written;

    }
#elif __linux__

    QString descriptor = ui->devicesComboBox->itemData(ui->devicesComboBox->currentIndex()).value<QString>();
    QStringList values = descriptor.split("_");
    if (values.size() == 2){

        ftdi_usb_open(ftdi, values.at(0).toInt(), values.at(0).toInt());

        ftdi_set_bitmode(ftdi, CSN | SCK | MOSI | CE, BITMODE_SYNCBB);

        state = CSN | CE;

        ftdi_write_data(ftdi, &state, 1);
    }
#endif

        readConfigFromDevice();

		qDebug() << "RX_PW_P0" << nrf24l01p_read_byte(RX_PW_P0);

		disable_irqs();
		set_rx_mode();
		power_up();

		timer->start();

    } else {
		timer->stop();
		power_down();

    }


    setUiEnabled(opened);


}




void MainWindow::onTestConnection(){
	unsigned char addr[5] = {0};
	unsigned char read[5] = {0};
	unsigned char olda[5] = {0};
	
	nrf24l01p_read(TX_ADDR, olda, sizeof(olda));
	for (int y = 0; y < 1000; y++){

        for (unsigned int i = 0; i < sizeof(addr); i++){
            addr[i] = qrand();
		}

		nrf24l01p_write(W_REGISTER | TX_ADDR, addr, sizeof(addr));
		nrf24l01p_read(TX_ADDR, read, sizeof(read));

		if (memcmp(addr, read, sizeof(addr))==0){
			qDebug() << y << ": ok (" << convertAddress(addr) << ")";
		} else {
			qDebug() << y << ": " << convertAddress(addr) << " : " << convertAddress(read);
		}

	}

	nrf24l01p_write(W_REGISTER | TX_ADDR, olda, sizeof(olda));


}


void MainWindow::readConfigFromDevice(){

    ui->rfChannelSpinBox->setValue( 2400 + get_channel());
    ui->lnaGain->setChecked(is_lna_gain_enabled());
    switch(get_rf_power()){
    case 0:
        ui->rfOutPower->setValue(-18);
        break;
    case 1:
        ui->rfOutPower->setValue(-12);
        break;
    case 2:
        ui->rfOutPower->setValue(-6);
        break;
    case 3:
        ui->rfOutPower->setValue(0);
        break;
    }

    if (get_speed() == 0){
        ui->speed1Mbps->setChecked(true);
    } else {
        ui->speed2Mbps->setChecked(true);
    }

    if (get_crc_size() == 0){
        ui->crc1Byte->setChecked(true);
    } else {
        ui->crc2Bytes->setChecked(true);
    }

    ui->crcGroupBox->setChecked(is_crc_enabled());

    int count = get_retry_count();
    int delay = (get_retry_delay() + 1) * 250;
    qDebug() << "count" << count << " : delay "<< delay;
    ui->autoRetransmitGroupBox->setChecked(count != 0);
    ui->retransmitCount->setValue(count);
    ui->retransmitDelay->setValue(delay);

    ui->dynPayloadLen->setChecked(is_dyn_size_enabled());

    unsigned char addr_tx[5];

    nrf24l01p_read(TX_ADDR, addr_tx, sizeof(addr_tx));
    ui->txAddress->setText(convertAddress(addr_tx));
    nrf24l01p_read(RX_ADDR_P0, addr_tx, sizeof(addr_tx));
    ui->rx0Address->setText(convertAddress(addr_tx));
    nrf24l01p_read(RX_ADDR_P1, addr_tx, sizeof(addr_tx));
    ui->rx1Address->setText(convertAddress(addr_tx));
    nrf24l01p_read(RX_ADDR_P2, addr_tx, 1);
    ui->rx2Address->setText(QString("%1").arg(addr_tx[0], 2, 16, QChar('0')).toUpper());
    nrf24l01p_read(RX_ADDR_P3, addr_tx, 1);
    ui->rx3Address->setText(QString("%1").arg(addr_tx[0], 2, 16, QChar('0')).toUpper());
    nrf24l01p_read(RX_ADDR_P4, addr_tx, 1);
    ui->rx4Address->setText(QString("%1").arg(addr_tx[0], 2, 16, QChar('0')).toUpper());
    nrf24l01p_read(RX_ADDR_P5, addr_tx, 1);
    ui->rx5Address->setText(QString("%1").arg(addr_tx[0], 2, 16, QChar('0')).toUpper());

	int addrWidth = get_address_width() + 2;
	QRadioButton *enRX= this->findChild<QRadioButton *>(QString("addressWidth%1").arg(addrWidth));
	if (enRX){
		enRX->setChecked(true);
		changeAddrMask(addrWidth);
	}
	
	for(int i = 0; i < 5; ++i)
	{
		QCheckBox *enRX= this->findChild<QCheckBox *>(QString("enRX%1").arg(i));
		enRX->setChecked(is_pipe_enabled(i));
		QCheckBox *autoAckRx= this->findChild<QCheckBox *>(QString("autoAckRx%1").arg(i));
		autoAckRx->setChecked(is_auto_ack_enabled(i));
		QCheckBox *fixedPacketSizeRx= this->findChild<QCheckBox *>(QString("fixedPacketSizeRx%1").arg(i));
		fixedPacketSizeRx->setChecked(!is_pipe_dyn_size_enabled(i));

		QSpinBox *packetSizeRx= this->findChild<QSpinBox *>(QString("packetSizeRx%1").arg(i));
		packetSizeRx->setEnabled(fixedPacketSizeRx->isChecked());
		packetSizeRx->setValue(get_pipe_size(i));
		
	}


}

QString MainWindow::convertAddress(unsigned char * addr){
    QString ret;
    for (int i = 0; i < 5; i++){
        QString number = QString("%1").arg(addr[i], 2, 16, QChar('0')).toUpper();
        ret += number;
        if (i < 4){
            ret += ":";
        }
    }
    return ret;
}

void MainWindow::addByte(QByteArray * array, unsigned char byte){

    array->append(state);

    for(int i = 0; i < 8; i++){
		bool isHIGH = (1 << (7-i)) & byte;
		bool isLast = i == 7;
		bool isNextHIGH = (!isLast &&  ((1 << (7-i-1)) & byte));
        if (isHIGH) {
            state |= MOSI;
        } else {
            state &= ~MOSI;
        }
		array->append(state);
        state |= SCK;
		array->append(state);
		if (!isNextHIGH || isLast){
			state &= ~MOSI;
		}
		//array->append(state);
		state &= ~SCK;
		array->append(state);

	}

    array->append(state);

}



void MainWindow::nrf24l01p_spi_rw(unsigned char *write, unsigned char *read, int size){

    QByteArray buff;


    state &= ~CSN;
    buff.append(state);


    for(int i = 0; i < size; i++){
        addByte(&buff, write[i]);
    }

    state |= CSN;
    buff.append(state);

    QByteArray buffRead(buff.length(), 0);


#ifdef _WIN32
    DWORD written = 0;
    FT_Write(ftHandle,(void*)buff.data(), buff.length(), &written);
    written = 0;
    FT_Read(ftHandle, (void*) buffRead.data(), buffRead.length(), &written);
#elif __linux__
    unsigned int written = 0;
    written = ftdi_write_data(ftdi, (unsigned char*)buff.data(), buff.length());
    ftdi_read_data(ftdi, (unsigned char*) buffRead.data(), buffRead.length());

#endif
    nrf24l01p_spi_decode((unsigned char*)buffRead.data(), written, read);

}

unsigned int MainWindow::nrf24l01p_spi_decode(unsigned char * inResponse, unsigned int inResponseSize, unsigned char * outBuff){
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


uint8_t MainWindow::nrf24l01p_read(uint8_t reg, uint8_t * mem, int size){
    unsigned char * write = (unsigned char*) malloc(size+1);
    unsigned char * read = (unsigned char*) malloc(size+1);
    memset(write, 0, size+1);
    memset(read, 0, size+1);
    write[0] = reg;
    nrf24l01p_spi_rw(write, read, size+1);
    memcpy(mem, read+1,size);
    unsigned char status = read[0];
    free(read);
    free(write);
    return status;
}
uint8_t MainWindow::nrf24l01p_write(uint8_t reg, uint8_t * mem, int size){
    unsigned char * write = (unsigned char*) malloc(size+1);
    unsigned char * read = (unsigned char*) malloc(size+1);
    memset(write, 0, size+1);
    memset(read, 0, size+1);
    write[0] = reg;
    memcpy(write+1, mem, size);
    nrf24l01p_spi_rw(write, read, size+1);
    unsigned char status = read[0];
    free(read);
    free(write);
    return status;
}

uint8_t MainWindow::nrf24l01p_read_byte(uint8_t reg){
    uint8_t buff;
    nrf24l01p_read(reg, &buff, sizeof(buff));
    return buff;
}

uint8_t MainWindow::nrf24l01p_write_byte(uint8_t reg, uint8_t value){
    uint8_t buff = value;
    return nrf24l01p_write(reg, &buff, sizeof(buff));
}

bool MainWindow::changeAddrMask( int width )
{
	QString mask, oldMask;
	switch(width){
		case 3:
			mask = "HH:HH:HH;0";
			break;
		case 4:
			mask = "HH:HH:HH:HH;0";
			break;
		case 5:
		default:
			mask = "HH:HH:HH:HH:HH;0";
			break;
	}

	oldMask = ui->txAddress->inputMask();
	ui->txAddress->setInputMask(mask);
	ui->rx0Address->setInputMask(mask);
	ui->rx1Address->setInputMask(mask);
	return mask.length() > oldMask.length();	
}


void MainWindow::onAddressWidthChanged(){
    QRadioButton * addressWidth = qobject_cast<QRadioButton*>(sender());
    QRegExp reg("(\\d+)");
    if (addressWidth && (reg.indexIn(addressWidth->objectName(), 0) != -1)){
        int width = reg.cap(1).toInt();
        qDebug() << "onAddressWidthChange" << addressWidth->objectName() << width;

        if (changeAddrMask(width)){

            unsigned char addr_tx[5];

            nrf24l01p_read(TX_ADDR, addr_tx, sizeof(addr_tx));
            ui->txAddress->setText(convertAddress(addr_tx));
            nrf24l01p_read(RX_ADDR_P0, addr_tx, sizeof(addr_tx));
            ui->rx0Address->setText(convertAddress(addr_tx));
            nrf24l01p_read(RX_ADDR_P1, addr_tx, sizeof(addr_tx));
            ui->rx1Address->setText(convertAddress(addr_tx));

        }

        set_address_width((width-2));

    }
}




QString MainWindow::getMaskFromRX1(){
    return "HH;0";
    QString rx1 = extractAddress(ui->rx1Address->text());
    if (rx1.length() > 2){

        QString rest;

        for (int i = 2; i < rx1.length(); i+=2){
            rest+= QString(":%1").arg(rx1.mid(i,2));
        }

        return QString("HH%1;0").arg(rest);
    } else {

    }
}

QString MainWindow::extractAddress(QString maskedAddress){
    return maskedAddress.remove(":");
}


void MainWindow::onLNAGain(bool state){
    if (state){
        lna_gain_enable();
    } else {
        lna_gain_disable();
    }
}

void MainWindow::onRFChannel(int value){
    power_down();
    set_channel((value-2400));
	power_up();
}

void MainWindow::onRFOutPower(int value){
    qDebug() << "onRFOutPower" << value;
     switch(value){
    case -18:
        set_rf_power(0);
        break;
    case -12:
        set_rf_power(1);
        break;
    case -6:
        set_rf_power(2);
        break;
    case 0:
        set_rf_power(3);
        break;
    }
}

void MainWindow::onSpeedChange(){
    if (ui->speed1Mbps->isChecked()){
        set_1mbps_speed();
    } else if (ui->speed2Mbps->isChecked()){
        set_2mbps_speed();
    }
}
void MainWindow::onCRCChanged(bool state){

	foreach(QCheckBox* autoAckRx, this->findChildren<QCheckBox*>(QRegExp("autoAckRx\\d"))){
		autoAckRx->setEnabled(state);
	}
	
    if (state){
		nrf24l01p_write_byte(W_REGISTER | EN_AA, oldEN_AA);
        crc_enable();
    } else {
		oldEN_AA = nrf24l01p_read_byte(R_REGISTER | EN_AA);
		nrf24l01p_write_byte(W_REGISTER | EN_AA, 0);
        crc_disable();
    }

	qDebug() << "crc:" << is_crc_enabled();
}
void MainWindow::onCRCSizeChange(){
    if (ui->crc1Byte->isChecked()){
        set_1byte_crc();
    } else if (ui->crc2Bytes->isChecked()){
        set_2byte_crc();
    }
}

void MainWindow::onAutoRetransmit(bool state){
    if (state){
        set_retry_count(3);
        ui->retransmitCount->setValue(3);
    } else {
        set_retry_count(0);
        ui->retransmitCount->setValue(3);
    }
}
void MainWindow::onRetransmitCount(int count){
    set_retry_count(count);
}

void MainWindow::onRetransmitDelay(int delay){
    set_retry_delay((delay-1));
}

#define swap(x) ( ((x & 0xFF) << 8) | ((x >> 8) & 0xFF) )

void MainWindow::checkNewPacket(){
    //qDebug() << "check";
    unsigned char fifo_status = 0, status = 0;
    status = nrf24l01p_read(FIFO_STATUS, &fifo_status, 1);

    //TODO while
    if ((fifo_status & (1 << RX_FIFO_EMPTY)) == 0){
       int pipe = (status  >> RX_P_NO) & 7;

       bool dynamicSize = ui->dynPayloadLen->isChecked();
       int pipe_size = dynamicSize ? get_dyn_packet_size() : nrf24l01p_read_byte(RX_PW_P0+pipe);

	   if (pipe_size > 32){
		   //bad case
			nrf24l01p_write(FLUSH_RX, 0, 0);
			return;
	   } 

       unsigned char buff[32] = {0};

       nrf24l01p_read(R_RX_PAYLOAD, buff, pipe_size);
       QString dump;
       for (int i = 0; i < pipe_size; i++){
           dump += QString("%1").arg(buff[i], 2, 16, QChar('0')).toUpper();
       }

       //unsigned short * stat = (unsigned short*) &buff;
       //float temp = buff[0] + (buff[1] ? 0.5 : 0);
       //qDebug() << "dump " << pipe << pipe_size << dump << temp << swap(stat[1]);

       int row = ui->tableWidget->rowCount();
       ui->tableWidget->insertRow(row);
       ui->tableWidget->setItem(row, 0, new QTableWidgetItem( QDateTime::currentDateTime().toString("hh:mm:ss.zzz")));
       ui->tableWidget->setItem(row, 1, new QTableWidgetItem( QString("%1").arg(pipe)));
       ui->tableWidget->setItem(row, 2, new QTableWidgetItem( QString("%1").arg(pipe_size)));
       ui->tableWidget->setItem(row, 3, new QTableWidgetItem( dump));
	   ui->tableWidget->scrollToBottom();


    }

}


void MainWindow::onPipeEnable(bool state){
    QCheckBox * enRX = qobject_cast<QCheckBox*>(sender());
    QRegExp reg("(\\d+)");
    if (enRX && (reg.indexIn(enRX->objectName(), 0) != -1)){
        int pipeId = reg.cap(1).toInt();
        qDebug() << "onPipeEnable" << state << enRX->objectName() << pipeId;
        if (state){
            pipe_enable(pipeId);
        } else {
            pipe_disable(pipeId);
        }
    }
}

void MainWindow::onAddressChange(){
    QLineEdit * rxAddress = qobject_cast<QLineEdit*>(sender());
    if (rxAddress){
        QString newAddress = rxAddress->displayText().remove(":");
        QByteArray binary = QByteArray::fromHex( QByteArray().append(newAddress) );
        QRegExp reg("(\\d+)");
        if (reg.indexIn(rxAddress->objectName(), 0) != -1){
            int pipeId = reg.cap(1).toInt();
            qDebug() << "onAddressChange" << newAddress  << binary.size() << rxAddress->objectName() << pipeId;
            nrf24l01p_write(W_REGISTER | (RX_ADDR_P0+pipeId), (unsigned char*)binary.data(), binary.size());
        } else {
            qDebug() << "onAddressChange" << newAddress  << binary.size() << rxAddress->objectName();
            nrf24l01p_write(W_REGISTER | TX_ADDR, (unsigned char*)binary.data(), binary.size());
        }
    }
}

void MainWindow::onAckEnable(bool state){
    QCheckBox * autoAckRx = qobject_cast<QCheckBox*>(sender());
    QRegExp reg("(\\d+)");
    if (autoAckRx && (reg.indexIn(autoAckRx->objectName(), 0) != -1)){
        int pipeId = reg.cap(1).toInt();
        qDebug() << "onAckEnable" << state << autoAckRx->objectName() << pipeId;
        if (state){
            auto_ack_enable(pipeId);
        } else {
            auto_ack_disable(pipeId);
        }
    }
}

void MainWindow::onFixedSizeEnable(bool state){
    QCheckBox * fixedPacketSizeRx = qobject_cast<QCheckBox*>(sender());
    QRegExp reg("(\\d+)");
    if (reg.indexIn(fixedPacketSizeRx->objectName(), 0) != -1){
        int pipeId = reg.cap(1).toInt();
        qDebug() << "onFixedSizeEnable" << state << fixedPacketSizeRx->objectName() << pipeId;
		QSpinBox *packetSizeRx= this->findChild<QSpinBox *>(QString("packetSizeRx%1").arg(pipeId));
		
		
        //TODO if (is_dyn_size_enabled()){}
        if (state){
			pipe_dyn_size_disable(pipeId);            
			int value = packetSizeRx->value();
			qDebug() << QString("new packet size %1 @ %2").arg(value).arg(pipeId);
			set_pipe_size(pipeId, value);
			
        } else {
			set_pipe_size(pipeId, 0);
			pipe_dyn_size_enable(pipeId);
        }
    }
}

void MainWindow::onPipeSizeChange(int newSize){
    QSpinBox * packetSizeRx = qobject_cast<QSpinBox*>(sender());
    QRegExp reg("(\\d+)");
    if (reg.indexIn(packetSizeRx->objectName(), 0) != -1){
        int pipeId = reg.cap(1).toInt();
        qDebug() << "onPipeSizeChange" << newSize << packetSizeRx->objectName() << pipeId;
        if (newSize >= 0 && newSize <= 32){
            set_pipe_size(pipeId, newSize);
        } else {
            qDebug() << "newSize out of range";
        }

    }
}






void MainWindow::onTabModeChange(int index){

	qDebug() << "mode" << index;
    power_down();
	if (index == 1){
		timer->stop();
		set_tx_mode();
	} else if(index == 0) {
		set_rx_mode();
		timer->start();
	}
	nrf24l01p_write(FLUSH_RX, 0, 0);
	nrf24l01p_write(FLUSH_TX, 0, 0);
    power_up();

}


void MainWindow::onSendPacket(){

    QString status = QString("Failed");
    QString text = ui->payloadData->text();
    QByteArray binary;

	if (text.isEmpty()) return;

    if (ui->hexPayload->isChecked()){
        binary = QByteArray::fromHex( QByteArray().append(text));
    } else {
        binary = text.toLocal8Bit();
    }
	
    if (ui->txAck->isChecked()){

        nrf24l01p_write(W_TX_PAYLOAD, (uint8_t*)binary.data(), binary.length());

		uint8_t fifo_st = 0, st = 0;
        do{
            st = nrf24l01p_read(R_REGISTER | FIFO_STATUS, &fifo_st, 1);
        } while( !is_tx_fifo_empty(fifo_st) && !is_max_retrys(st) );

        if (is_max_retrys(st)){
			nrf24l01p_write(FLUSH_TX, 0, 0);
            nrf24l01p_write_byte(W_REGISTER | STATUS, st & 0x70); // clear status
        } else {
            status = QString("OK");
        }


    } else {
        nrf24l01p_write(W_TX_PAYLOAD_NOACK, (uint8_t*)binary.data(), binary.length());
        status = QString("Noack");
    }



	int row = ui->tableWidget_2->rowCount();
	ui->tableWidget_2->insertRow(row);
    ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")));
    ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(ui->txAddress->text()));
    ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(QString("%1(%2)").arg(text).arg(binary.length())));
    ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(status));
	ui->tableWidget_2->scrollToBottom();


}

void MainWindow::save()
{
	QSettings settings("settings.conf", QSettings::NativeFormat);
	settings.sync();
	foreach(QString key, settings.allKeys()){
		qDebug() << key << settings.value(key);
	}
	foreach(QCheckBox* tb, findChildren<QCheckBox*>()){
		settings.setValue(tb->objectName(), tb->isChecked());
	}
	foreach(QLineEdit * tb, findChildren<QLineEdit *>()){
		settings.setValue(tb->objectName(), tb->text());
	}
	foreach(QRadioButton * tb, findChildren<QRadioButton *>()){
		settings.setValue(tb->objectName(), tb->isChecked());
	}
	foreach(QSpinBox * tb, findChildren<QSpinBox *>()){
		settings.setValue(tb->objectName(), tb->value());
	}
	settings.sync();
	
	
		

}

void MainWindow::setUiEnabled(bool enabled)
{
    ui->adressesGroupBox->setEnabled(enabled);
    ui->settingsGroupBox->setEnabled(enabled);
    ui->tabWidget->setEnabled(enabled);

	ui->actionPinout->setEnabled(!enabled);

	ui->actionLoad->setEnabled(enabled);
	ui->actionSave->setEnabled(enabled);
	ui->actionTestConnection->setEnabled(enabled);
	ui->actionMemDump->setEnabled(enabled);
}

void MainWindow::configPinouts()
{
	FTDIPinout * pinoutDialog = new FTDIPinout(this);
	pinoutDialog->initPins(CSN, SCK, MOSI, MISO, CE);
	pinoutDialog->setModal(true);
	qDebug() << "before show";
	int val = pinoutDialog->exec();
	if (val){
			qDebug() << "MISO" << MISO;
			MISO = pinoutDialog->getMISO();
			qDebug() << "MISO" << MISO;
	}

    //pinoutDialog->show();
	qDebug() << "after show" << val;
}

void MainWindow::onDynPayloadLen(bool state)
{
	
	
	/*
	power_down();
	nrf24l01p_write_byte(ACTIVATE, 0x73);
	power_up();
    */

    if (state){
        dyn_size_enable();
    } else {
        dyn_size_disable();
    }
	power_down();
	nrf24l01p_write(FLUSH_RX, 0, 0);
	nrf24l01p_write(FLUSH_TX, 0, 0);
	power_up();
	
}

void MainWindow::onHexChanged(bool state)
{
    if (state){
        ui->payloadData->setValidator(new HexValidator(this));
        ui->payloadData->setText(QString::fromUtf8(ui->payloadData->text().toUtf8().toHex().constData()));
    } else {
        ui->payloadData->setValidator(0);
        ui->payloadData->setText(QString::fromUtf8(QByteArray::fromHex( QByteArray().append(ui->payloadData->text())).constData()));
    }
}

void MainWindow::scanDevices()
{

    ui->devicesComboBox->clear();

#ifdef _WIN32

    DWORD version = 0;
    if (FT_GetLibraryVersion(&version) == FT_OK){
        qDebug() << "GetLibraryVersion: " <<  QString("%1").arg(version, 0, 16);;
    }

    DWORD numDevs = 0;
    if (FT_CreateDeviceInfoList(&numDevs) == FT_OK){
        if (numDevs){
            devlist = (FT_DEVICE_LIST_INFO_NODE *)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
            if (FT_GetDeviceInfoList(devlist, &numDevs) == FT_OK){
                for (int i = 0; i < (int)numDevs; i++){
                    if (devlist[i].Type == FT_DEVICE_232R){
                        ui->devicesComboBox->addItem(QString("%1 / %2").arg(devlist[i].Description).arg(devlist[i].SerialNumber), i);
                        qDebug() << devlist[i].ftHandle;
                    }
                }
            }
            free(devlist);
        }
    }

#elif __linux__

    if (!ftdi){
        ftdi = ftdi_new();
    }

    if (ftdi){
        struct ftdi_device_list *devlist,  *curdev;
        int devices = ftdi_usb_find_all(ftdi, &devlist, 0, 0);
        if (devices > 0){
            char manufacturer[128], description[128], serial[128];
            int i;
            for (curdev = devlist; curdev != NULL; i++){

                ftdi_usb_get_strings(ftdi, curdev->dev, manufacturer, sizeof(manufacturer)-1, description, sizeof(description)-1, serial, sizeof(serial)-1);
                QString descr = QString("%1_%2").arg(curdev->dev->descriptor.idVendor),arg(curdev->dev->descriptor.idProduct);
                ui->devicesComboBox->addItem(QString("%1 / %2").arg(description).arg(serial), descr);
                curdev = curdev->next;
            }
            ftdi_list_free(&devlist);
        }

    }


#endif
	if (!timer->isActive()){
		ui->actionConnect->setEnabled(ui->devicesComboBox->count() > 0);
	}

}















