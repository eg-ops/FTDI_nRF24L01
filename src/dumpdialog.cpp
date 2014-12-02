#include "dumpdialog.h"
#include "ui_dumpdialog.h"

DumpDialog::DumpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DumpDialog)
{
    ui->setupUi(this);
}

DumpDialog::~DumpDialog()
{
    delete ui;
}
