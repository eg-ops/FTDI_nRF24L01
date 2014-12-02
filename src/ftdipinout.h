#ifndef FTDIPINOUT_H
#define FTDIPINOUT_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QSettings>
#include "ui_FTDI_Pinout.h"

class FTDIPinout : public QDialog
{
	Q_OBJECT

public:
	FTDIPinout(QWidget *parent);
	~FTDIPinout();

private:
	Ui::Dialog ui;
    QMap<int, QString> FT232RL_pinName;
    QMap<int, int> FT232RL_mapping;
    QMap<int, QString> FT232RQ_pinName;
    QMap<int, int> FT232RQ_mapping;
    QList<int> available;
    QList<QComboBox *> pins;
    QList<QToolButton *> resets;
    QMap<QComboBox *, int> prevPin;


    QSettings settings;
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

};

#endif // FTDIPINOUT_H
