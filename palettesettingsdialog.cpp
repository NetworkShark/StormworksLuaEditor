#include "palettesettingsdialog.h"
#include "ui_palettesettingsdialog.h"

PaletteSettingsDialog::PaletteSettingsDialog(QWidget *parent, std::map<QString, QString> *palette) :
    QDialog(parent),
    ui(new Ui::PaletteSettingsDialog)
{
    ui->setupUi(this);
    this->dictionaryPalette = palette;
    if (this->dictionaryPalette)
        Init();
}

PaletteSettingsDialog::~PaletteSettingsDialog()
{
    delete ui;
}

void PaletteSettingsDialog::Init()
{
    QList<QPushButton *> btns = findChildren<QPushButton *>();
    QRegExp regex("btnPicker([A-z_0-9]+)");
    foreach (QPushButton* btn, btns) {
        if (regex.exactMatch(btn->objectName()))
        {
            std::map<QString, QString>::iterator it;
            if ((it = dictionaryPalette->find(regex.cap(1))) != dictionaryPalette->end())
            {
                DisplayColor(btn, it->second);
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
