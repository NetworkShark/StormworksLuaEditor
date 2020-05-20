#pragma once

#include <QDialog>
#include <QPainter>
#include <QInputDialog>
#include <QMessageBox>
#include <QColorDialog>
#include "structs.h"

namespace Ui {
class DialogHighlight;
}

class DialogHighlight : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHighlight(QWidget *parent = nullptr, QString *title = nullptr);
    ~DialogHighlight();
    KeywordHighlight showDialog(QString *nameRule, KeywordHighlight *rule = nullptr);

private slots:
    void on_listSearchWords_itemSelectionChanged();
    void on_pushButtonAdd_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonRemove_clicked();
    void on_btnPickerColor_clicked();

private:
    Ui::DialogHighlight *ui;
    KeywordHighlight *rule;
    void DisplayColor(QPushButton *btn, QString keyColor, const int marginW = 12, const int marginH = 12, const char *borderColor = "#a3a3a3");
    void on_textBoxRegex_textChanged();
    void on_textBoxName_textChanged();
};

