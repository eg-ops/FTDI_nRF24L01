#ifndef FTDIPINOUT_H
#define FTDIPINOUT_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QSettings>
#include "ui_FTDI_Pinout.h"


#define CSN_DEFAULT_BIT_INDEX 0
#define SCK_DEFAULT_BIT_INDEX 1
#define MOSI_DEFAULT_BIT_INDEX 2
#define MISO_DEFAULT_BIT_INDEX 3
#define CE_DEFAULT_BIT_INDEX 4

class FTDIPinout : public QDialog
{
	Q_OBJECT

public:
	FTDIPinout(QWidget *parent);
	~FTDIPinout();

private:
	Ui::Dialog ui;
    QMap<int, QString> FT232RL_pinName;
    QMap<int, int> FT232RL_mapping; // bit to pin
    QMap<int, QString> FT232RQ_pinName;
    QMap<int, int> FT232RQ_mapping; // bit to pin
    QList<int> available;
    QList<QComboBox *> pins;
    QList<QToolButton *> resets;
    QMap<QComboBox *, int> prevPin;


    QSettings settings;

	int toBitIndex(unsigned char val);
    int getBit(QComboBox * cbox, int def);
    void isAllPinsSet();

public:
    int getCE();
    int getSCK();
    int getMOSI();
    int getMISO();
    int getCSN();

public slots:;

    void onChipChange(int index);
    void pinAcquire(int index);
    void pinReleased();
    void save();
	void initPins(unsigned char CSN, unsigned char SCK, unsigned char MOSI, unsigned char MISO, unsigned char CE);
};

#endif // FTDIPINOUT_H
