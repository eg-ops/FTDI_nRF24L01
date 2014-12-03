#include "hexvalidator.h"

#include <QApplication>


HexValidator::HexValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State HexValidator::validate(QString &input, int &pos) const
{
    // match against needed regexp
    QRegExp rx("([0-9A-Fa-f]{2}[ ,.:]{0,1})*[0-9A-Fa-f]{0,1}$");
    if (rx.exactMatch(input)) {
        return QValidator::Acceptable;
    }

    QApplication::beep();

    return QValidator::Invalid;
}
