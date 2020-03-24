#ifndef PALETTESETTINGSDIALOG_H
#define PALETTESETTINGSDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QPainter>
#include <map>

namespace Ui {
class PaletteSettingsDialog;
}

class PaletteSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaletteSettingsDialog(QWidget *parent = nullptr, std::map<QString, QString> *palette = nullptr);
    ~PaletteSettingsDialog();

private:
    Ui::PaletteSettingsDialog *ui;
    std::map<QString, QString> *dictionaryPalette;
    void Init();
    void DisplayColor(QPushButton* btn, QString keyColor, const int marginW = 12, const int marginH = 12, const char* borderColor = "#a3a3a3");

};

#endif // PALETTESETTINGSDIALOG_H
