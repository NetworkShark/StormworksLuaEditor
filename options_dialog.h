#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>

namespace Ui {
class options_Dialog;
}

class options_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit options_Dialog(QWidget *parent = nullptr);
    ~options_Dialog();

private:
    Ui::options_Dialog *ui;
};

#endif // OPTIONS_DIALOG_H
