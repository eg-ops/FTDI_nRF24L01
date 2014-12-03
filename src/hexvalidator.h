#ifndef HEXVALIDATOR_H
#define HEXVALIDATOR_H

#include <QValidator>

class HexValidator : public QValidator
{
    Q_OBJECT
public:
    explicit HexValidator(QObject *parent = 0);
    QValidator::State validate(QString &input, int &pos) const;
signals:

public slots:

};

#endif // HEXVALIDATOR_H
