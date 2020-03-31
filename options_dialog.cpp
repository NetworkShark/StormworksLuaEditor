#include "options_dialog.h"
#include "ui_options_dialog.h"

options_Dialog::options_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::options_Dialog)
{
    ui->setupUi(this);
}

options_Dialog::~options_Dialog()
{
    delete ui;
}
