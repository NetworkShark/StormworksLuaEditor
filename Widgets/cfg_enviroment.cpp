#include "cfg_enviroment.h"
#include "ui_cfg_enviroment.h"

cfg_Enviroment::cfg_Enviroment(QWidget *parent, Settings *setting) :
    QWidget(parent),
    ui(new Ui::cfg_Enviroment)
{
    ui->setupUi(this);
    this->settings = setting;
    QFontDatabase dbFont;
    QStringList families = dbFont.families();
    for (QString family : families) {
        if (dbFont.pointSizes(family).length() > 0)
            ui->comboBoxFontFamily->addItem(family, family);
    }
    QList<int> sizes = dbFont.pointSizes(families.first());
    ui->comboBoxFontSize->clear();
    for (int i=0;i<sizes.length();i++) {
        ui->comboBoxFontSize->addItem(QString("%1").arg(sizes[i]), sizes[i]);
    }
    if (settings) {
        map<QString, KeywordHighlight>* highlightings = &settings->fontSettings.highlightings;
        prepare_listWidgetColorScheme_Colors(ui->listWidgetColorScheme, highlightings);

        int comboIdx = -1;
        if ((comboIdx = ui->comboBoxFontFamily->findData(settings->fontSettings.fontFamily)) != -1)
            ui->comboBoxFontFamily->setCurrentIndex(comboIdx);
        else
            ui->comboBoxFontFamily->setCurrentIndex(0);
        comboIdx = -1;
        if ((comboIdx = ui->comboBoxFontSize->findData(settings->fontSettings.fontSize)) != -1)
            ui->comboBoxFontSize->setCurrentIndex(comboIdx);
        else
            ui->comboBoxFontSize->setCurrentIndex(0);

    }
    setup = false;
}

cfg_Enviroment::~cfg_Enviroment()
{
    delete ui;
}

void cfg_Enviroment::on_listWidgetColorScheme_itemSelectionChanged()
{
    if (setup) return;
    QListWidget* foundWidget = findChild<QListWidget*>("listWidgetColorScheme");
    bool enable = foundWidget->selectedItems().count() > 0;
    QPushButton* foundButton = findChild<QPushButton*>("buttonEdit");
    foundButton->setEnabled(enable);
    foundButton = findChild<QPushButton*>("buttonDelete");
    foundButton->setEnabled(enable);
}

void cfg_Enviroment::on_buttonNew_clicked()
{
    if (setup) return;
    DialogHighlight *dialog = new DialogHighlight(this);
    QString ruleName = "";
    KeywordHighlight newRule = dialog->showDialog(&ruleName);
    settings->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>(ruleName, newRule));
    map<QString, KeywordHighlight>* highlightings = &settings->fontSettings.highlightings;
    QListWidget* listWidgetColorScheme = findChild<QListWidget*>("listWidgetColorScheme");
    listWidgetColorScheme->clear();
    prepare_listWidgetColorScheme_Colors(listWidgetColorScheme, highlightings);
}

void cfg_Enviroment::on_buttonEdit_clicked()
{
    if (setup) return;
    QListWidget* foundWidget = findChild<QListWidget*>("listWidgetColorScheme");
    QListWidgetItem* item = foundWidget->selectedItems().first();
    openEditRuleDialog(item);
}

void cfg_Enviroment::on_buttonDelete_clicked()
{
    if (setup) return;
    QListWidget* foundWidget = findChild<QListWidget*>("listWidgetColorScheme");
    QListWidgetItem* item = foundWidget->selectedItems().first();
    int res = QMessageBox::warning(this, tr("Delete Rule"), tr("Are you sure to delete this rule?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (res == QMessageBox::Yes)
    {
        settings->fontSettings.highlightings.erase(item->text());
        foundWidget->takeItem(foundWidget->row(item));
        item->setSelected(false);
    }

}

void cfg_Enviroment::on_listWidgetColorScheme_itemDoubleClicked(QListWidgetItem *item)
{
    if (setup) return;
    openEditRuleDialog(item);
}

void cfg_Enviroment::on_comboBoxFontFamily_currentIndexChanged(int index)
{
    if (setup) return;
    this->settings->fontSettings.fontFamily = ui->comboBoxFontFamily->itemText(index);
}

void cfg_Enviroment::on_comboBoxFontSize_currentIndexChanged(int index)
{
    if (setup) return;
    this->settings->fontSettings.fontSize = ui->comboBoxFontSize->itemData(index).toInt();
}

void cfg_Enviroment::openEditRuleDialog(QListWidgetItem *item)
{
    QString name = item->text();
    QString title = QString("Edit Rule <%1>").arg(name);
    KeywordHighlight conf = settings->fontSettings.highlightings[name];
    if (conf.defaultColor)
    {
        QColorDialog dialog(item->background().color(), this);
        dialog.show();
        dialog.exec();
        if (dialog.result() == QDialog::Accepted) {
            QColor result = dialog.selectedColor();
            prepare_listWidgetColorScheme_Colors(item, result);
            settings->fontSettings.highlightings[name] = result.name();
        }
    }
    else
    {
        DialogHighlight *dialog = new DialogHighlight(this, &title);
        QString newName(name);
        KeywordHighlight newRule = dialog->showDialog(&newName, &conf);
        if (newRule != conf)
                    settings->fontSettings.highlightings[name] = newRule;
        if (newName != name)
        {
            newRule = settings->fontSettings.highlightings[name];
            settings->fontSettings.highlightings.erase(name);
            settings->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>(newName, newRule));
        }
    }
    item->setSelected(false);
    map<QString, KeywordHighlight>* highlightings = &settings->fontSettings.highlightings;
    QListWidget* listWidgetColorScheme = findChild<QListWidget*>("listWidgetColorScheme");
    listWidgetColorScheme->clear();
    prepare_listWidgetColorScheme_Colors(listWidgetColorScheme, highlightings);
}

void cfg_Enviroment::prepare_listWidgetColorScheme_Colors(QListWidgetItem* item, QColor color)
{
    item->setBackground(QBrush(color));
    float colorPerc = ( 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue())/255;
    if (colorPerc < 0.5)
        item->setForeground(QBrush(QColor(255, 255, 255)));
    else
        item->setForeground(QBrush(QColor(0, 0, 0)));
}

void cfg_Enviroment::prepare_listWidgetColorScheme_Colors(QListWidget* listWidget, map<QString, KeywordHighlight> *items)
{
    for (map<QString, KeywordHighlight>::iterator item=items->begin();item!=items->end();++item) {
        QListWidgetItem* itemList = new QListWidgetItem(item->first, nullptr, 0);
        QColor color(item->second.color);
        prepare_listWidgetColorScheme_Colors(itemList, color);
        listWidget->addItem(itemList);
    }
}
