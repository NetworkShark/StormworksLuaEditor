#ifndef CFG_ENVIROMENT_H
#define CFG_ENVIROMENT_H

#include <QWidget>
#include <QListWidgetItem>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDatabase>

#include "structs.h"
#include "Forms/dialoghighlight.h"

namespace Ui {
class cfg_Enviroment;
}

class cfg_Enviroment : public QWidget
{
    Q_OBJECT

public:
    Settings *settings;
    explicit cfg_Enviroment(QWidget *parent = nullptr, Settings *setting = nullptr);
    ~cfg_Enviroment();

public slots:
    void on_listWidgetColorScheme_itemDoubleClicked(QListWidgetItem *item);
    void on_buttonNew_clicked();
    void on_buttonEdit_clicked();
    void on_buttonDelete_clicked();
    void on_listWidgetColorScheme_itemSelectionChanged();
    void on_comboBoxFontFamily_currentIndexChanged(int index);
    void on_comboBoxFontSize_currentIndexChanged(int index);

private:
    bool setup = true;
    Ui::cfg_Enviroment *ui;
    void prepare_listWidgetColorScheme_Colors(QListWidgetItem* item, QColor color);
    void prepare_listWidgetColorScheme_Colors(QListWidget* listWidget, map<QString, KeywordHighlight> *items);
    void openEditRuleDialog(QListWidgetItem *item);
};

#endif // CFG_ENVIROMENT_H
