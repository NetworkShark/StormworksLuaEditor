#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include "structs.h"
#include "dialoghighlight.h"

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

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidgetColorScheme_itemDoubleClicked(QListWidgetItem *item);
    void on_buttonNew_clicked();
    void on_buttonEdit_clicked();
    void on_buttonDelete_clicked();
    void on_listWidgetColorScheme_itemSelectionChanged();

private:
    Ui::OptionsDialog *ui;
    void prepare_listWidgetColorScheme_Colors(QListWidgetItem* item, QColor color);
    void prepare_listWidgetColorScheme_Colors(QListWidget* listWidget, map<QString, KeywordHighlight> *items);
    void openEditRuleDialog(QListWidgetItem *item);
    void accept() override;
};

#endif // OPTIONSDIALOG_H
