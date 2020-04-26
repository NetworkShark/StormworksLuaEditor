#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <sstream>

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

void OptionsDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->labelItemInUse->setText(item->text());
    for (int i=ui->tabWidget->count();i>0;i--)
    {
        ui->tabWidget->widget(i-1)->deleteLater();
    }
    ui->tabWidget->clear();
    if (item->text() == "Enviroment") {
        QWidget* newTab = new QWidget();
        QGroupBox* groupBoxFont = new QGroupBox(newTab);
        groupBoxFont->setObjectName(QString::fromUtf8("groupBoxFont"));
        groupBoxFont->setGeometry(QRect(9, 9, 471, 70));
        groupBoxFont->setTitle("Font");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBoxFont->sizePolicy().hasHeightForWidth());
        groupBoxFont->setSizePolicy(sizePolicy);
        groupBoxFont->setMinimumSize(QSize(471, 70));
        groupBoxFont->setMaximumSize(QSize(471, 70));
        QLabel* label = new QLabel(groupBoxFont);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 20, 60, 41));
        label->setAlignment(Qt::AlignCenter);
        label->setText("Family:");
        QComboBox* comboBoxFontFamily = new QComboBox(groupBoxFont);
        comboBoxFontFamily->addItem(QString());
        comboBoxFontFamily->setObjectName(QString::fromUtf8("comboBox"));
        comboBoxFontFamily->setGeometry(QRect(50, 30, 170, 21));
        QLabel* labelSize = new QLabel(groupBoxFont);
        labelSize->setObjectName(QString::fromUtf8("labelSize"));
        labelSize->setGeometry(QRect(230, 20, 50, 41));
        labelSize->setAlignment(Qt::AlignCenter);
        labelSize->setText("Size:");
        QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
        spinBoxFontSize->setObjectName(QString::fromUtf8("spinBox"));
        spinBoxFontSize->setGeometry(QRect(270, 30, 42, 22));
        QGroupBox* groupBoxColorScheme = new QGroupBox(newTab);
        groupBoxColorScheme->setObjectName(QString::fromUtf8("groupBoxColorScheme"));
        groupBoxColorScheme->setEnabled(true);
        groupBoxColorScheme->setGeometry(QRect(10, 90, 471, 241));
        groupBoxColorScheme->setTitle("Color Scheme");
        sizePolicy.setHeightForWidth(groupBoxColorScheme->sizePolicy().hasHeightForWidth());
        groupBoxColorScheme->setSizePolicy(sizePolicy);
        groupBoxColorScheme->setMinimumSize(QSize(471, 241));
        QListWidget* listWidgetColorScheme = new QListWidget(groupBoxColorScheme);
        listWidgetColorScheme->setObjectName(QString::fromUtf8("listWidgetColorScheme"));
        listWidgetColorScheme->setGeometry(QRect(10, 20, 320, 210));
        listWidgetColorScheme->setSelectionMode(QAbstractItemView::SingleSelection);
        if (settings) {
            map<QString, KeywordHighlight>* highlightings = &settings->fontSettings.highlightings;
            prepare_listWidgetColorScheme_Colors(listWidgetColorScheme, highlightings);
        }
        listWidgetColorScheme->setStyleSheet("QListWidget::item:selected { background-color: #3333FF; }");
        connect(listWidgetColorScheme, &QListWidget::itemDoubleClicked, this, &OptionsDialog::on_listWidgetColorScheme_itemDoubleClicked);
        connect(listWidgetColorScheme, &QListWidget::itemSelectionChanged, this, &OptionsDialog::on_listWidgetColorScheme_itemSelectionChanged);
        QWidget* verticalLayoutWidget = new QWidget(groupBoxColorScheme);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(360, 30, 100, 201));
        QVBoxLayout* buttonSchemeLayout = new QVBoxLayout(verticalLayoutWidget);
        buttonSchemeLayout->setObjectName(QString::fromUtf8("buttonSchemeLayout"));
        buttonSchemeLayout->setContentsMargins(0, 0, 0, 0);
        QPushButton* buttonNew = new QPushButton(verticalLayoutWidget);
        buttonNew->setObjectName(QString::fromUtf8("buttonNew"));
        buttonNew->setText("New");
        buttonSchemeLayout->addWidget(buttonNew);
        connect(buttonNew, &QPushButton::clicked, this, &OptionsDialog::on_buttonNew_clicked);
        QPushButton* buttonEdit = new QPushButton(verticalLayoutWidget);
        buttonEdit->setObjectName(QString::fromUtf8("buttonEdit"));
        buttonEdit->setEnabled(false);
        buttonEdit->setText("Edit");
        buttonSchemeLayout->addWidget(buttonEdit);
        connect(buttonEdit, &QPushButton::clicked, this, &OptionsDialog::on_buttonEdit_clicked);
        QPushButton* buttonDelete = new QPushButton(verticalLayoutWidget);
        buttonDelete->setObjectName(QString::fromUtf8("buttonDelete"));
        buttonDelete->setEnabled(false);
        buttonDelete->setText("Delete");
        buttonSchemeLayout->addWidget(buttonDelete);
        connect(buttonDelete, &QPushButton::clicked, this, &OptionsDialog::on_buttonDelete_clicked);
        ui->tabWidget->addTab(newTab, "General");
    }
}

void OptionsDialog::on_listWidgetColorScheme_itemSelectionChanged()
{
    QListWidget* foundWidget = findChild<QListWidget*>("listWidgetColorScheme");
    bool enable = foundWidget->selectedItems().count() > 0;
    QPushButton* foundButton = findChild<QPushButton*>("buttonEdit");
    foundButton->setEnabled(enable);
    foundButton = findChild<QPushButton*>("buttonDelete");
    foundButton->setEnabled(enable);
}

void OptionsDialog::on_buttonNew_clicked()
{
    DialogHighlight *dialog = new DialogHighlight(this);
    QString ruleName = "";
    KeywordHighlight newRule = dialog->showDialog(&ruleName);
    settings->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>(ruleName, newRule));
    map<QString, KeywordHighlight>* highlightings = &settings->fontSettings.highlightings;
    QListWidget* listWidgetColorScheme = findChild<QListWidget*>("listWidgetColorScheme");
    listWidgetColorScheme->clear();
    prepare_listWidgetColorScheme_Colors(listWidgetColorScheme, highlightings);
}

void OptionsDialog::on_buttonEdit_clicked()
{
    QListWidget* foundWidget = findChild<QListWidget*>("listWidgetColorScheme");
    QListWidgetItem* item = foundWidget->selectedItems().first();
    openEditRuleDialog(item);
}

void OptionsDialog::on_buttonDelete_clicked()
{
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

void OptionsDialog::on_listWidgetColorScheme_itemDoubleClicked(QListWidgetItem *item)
{
    openEditRuleDialog(item);
}

void OptionsDialog::openEditRuleDialog(QListWidgetItem *item)
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

void OptionsDialog::prepare_listWidgetColorScheme_Colors(QListWidgetItem* item, QColor color)
{
    item->setBackground(QBrush(color));
    float colorPerc = ( 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue())/255;
    if (colorPerc < 0.5)
        item->setForeground(QBrush(QColor(255, 255, 255)));
    else
        item->setForeground(QBrush(QColor(0, 0, 0)));
}

void OptionsDialog::prepare_listWidgetColorScheme_Colors(QListWidget* listWidget, map<QString, KeywordHighlight> *items)
{
    for (map<QString, KeywordHighlight>::iterator item=items->begin();item!=items->end();++item) {
        QListWidgetItem* itemList = new QListWidgetItem(item->first, nullptr, 0);
        QColor color(item->second.color);
        prepare_listWidgetColorScheme_Colors(itemList, color);
        listWidget->addItem(itemList);
    }
}

void OptionsDialog::accept()
{
    this->settings->writeConfig();
    QDialog::accept();
}
