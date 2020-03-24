#include "palettesettingsdialog.h"
#include "ui_palettesettingsdialog.h"

PaletteSettingsDialog::PaletteSettingsDialog(QWidget *parent, Settings *globalSettings) :
    QDialog(parent),
    ui(new Ui::PaletteSettingsDialog)
{
    ui->setupUi(this);

    if (globalSettings) {
        this->globalSettings = globalSettings;
        Init();
    }
}

PaletteSettingsDialog::~PaletteSettingsDialog()
{
    delete ui;
}

void PaletteSettingsDialog::Init()
{
    QRegExp regex("btnPicker([A-z_0-9]+)");
    QList<QPushButton *> btns = findChildren<QPushButton *>(regex);
    foreach (QPushButton* btn, btns) {
        if (regex.exactMatch(btn->objectName()))
        {
            QString color = getColorfromDictionary(regex.cap(1));
            if (color != nullptr) {
                DisplayColor(btn, color);
                connect(btn, &QPushButton::clicked, this, &PaletteSettingsDialog::on_btnPicker_clicked);
            }
        }
    }
}

void PaletteSettingsDialog::DisplayColor(QPushButton* btn, QString keyColor, const int marginW, const int marginH, const char* borderColor)
{
    QPixmap* pix = new QPixmap(btn->width() - marginW, btn->height() - marginH);
    QRect colorRect(2, 2, pix->size().width() - 4, pix->size().height() - 4);
    QColor color(keyColor);
    QPainter painter(pix);

    painter.fillRect(colorRect, color);
    painter.setPen(QColor(borderColor));
    painter.drawRect(0, 0, pix->size().width() - 1, pix->size().height() - 1);

    QIcon ButtonIcon(pix->copy());
    btn->setIcon(ButtonIcon);
    btn->setIconSize(pix->rect().size());
}

QString PaletteSettingsDialog::getColorfromDictionary(QString key)
{
    std::map<QString, KeywordHighlight>::iterator it;
    if ((it = globalSettings->highlightings.find(key)) != globalSettings->highlightings.end())
    {
        return it->second.color;
    }
    return nullptr;
}

bool PaletteSettingsDialog::setColorfromDictionary(QString key, QColor color)
{
    std::map<QString, KeywordHighlight>::iterator it;
    if ((it = globalSettings->highlightings.find(key)) != globalSettings->highlightings.end())
    {
        QString colorName =  color.name();
        globalSettings->highlightings[key] = colorName;
        return true;
    }
    return false;
}

void PaletteSettingsDialog::on_btnPicker_clicked()
{
    QRegExp regex("btnPicker([A-z_0-9]+)");
    if (regex.exactMatch(this->sender()->objectName())) {
        QString key = regex.cap(1);
        QString color = getColorfromDictionary(regex.cap(1));
        if (color != nullptr) {
            QColorDialog dialog(color, this);
            /*dialog.show();
            dialog.exec();*/
            QColor c = dialog.getColor(color, this, regex.cap(1));
            if (setColorfromDictionary(regex.cap(1), c))
            {
                DisplayColor((QPushButton*)this->sender(), c.name());
            }
        }
    }
}
