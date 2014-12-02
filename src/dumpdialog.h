#ifndef DUMPDIALOG_H
#define DUMPDIALOG_H

#include <QDialog>

namespace Ui {
class DumpDialog;
}

class DumpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DumpDialog(QWidget *parent = 0);
    ~DumpDialog();

private:
    Ui::DumpDialog *ui;
};

#endif // DUMPDIALOG_H
