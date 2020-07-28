#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::accept()
{
    this->settings->writeConfig();
    QDialog::accept();
}

int OptionsDialog::exec()
{
    on_listWidget_itemClicked(ui->listWidget->item(0));
    return QDialog::exec();
}

void OptionsDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->labelItemInUse->setText(item->text());
    if (item->text() == "Enviroment") {
        cfg_Enviroment* cfg = new cfg_Enviroment(nullptr, settings);
        cfg->setMinimumSize(550, 360);
        ui->verticalLayout_2->addWidget(cfg);
    }
}
