#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

#include "structs.h"
#include "Widgets/cfg_enviroment.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    Settings *settings;
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();
    int exec() override;

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::OptionsDialog *ui;
    void accept() override;
};

#endif // OPTIONSDIALOG_H
