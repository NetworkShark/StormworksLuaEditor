#include "dialoghighlight.h"
#include "ui_dialoghighlight.h"

DialogHighlight::DialogHighlight(QWidget *parent, QString *title) :
    QDialog(parent),
    ui(new Ui::DialogHighlight)
{
    ui->setupUi(this);

    if (title && title->count())
        this->setWindowTitle(*title);
    else
        this->setWindowTitle("New Highlight Rule");

    ui->comboBoxFontWeight->clear();
    ui->comboBoxFontWeight->addItem("Thin", 0);
    ui->comboBoxFontWeight->addItem("ExtraLight", 12);
    ui->comboBoxFontWeight->addItem("Light", 25);
    ui->comboBoxFontWeight->addItem("Normal", 50);
    ui->comboBoxFontWeight->addItem("Medium", 57);
    ui->comboBoxFontWeight->addItem("DemiBold", 63);
    ui->comboBoxFontWeight->addItem("Bold", 75);
    ui->comboBoxFontWeight->addItem("ExtraBold", 81);
    ui->comboBoxFontWeight->addItem("Black", 87);
}

DialogHighlight::~DialogHighlight()
{
    delete ui;
}

void DialogHighlight::DisplayColor(QPushButton* btn, QString keyColor, const int marginW, const int marginH, const char* borderColor)
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

void DialogHighlight::on_listSearchWords_itemSelectionChanged()
{
    if ((ui->listSearchWords->selectedItems().count() <= 0) ||
            (ui->listSearchWords->selectedItems().count() > 1))
    {
        ui->pushButtonEdit->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
    }
    else
    {
        ui->pushButtonEdit->setEnabled(true);
        ui->pushButtonRemove->setEnabled(true);
    }
}

void DialogHighlight::on_pushButtonAdd_clicked()
{
    bool ok = false;
    QString text = NULL;
    if (hasRegex)
        text = QInputDialog::getText(this, tr("Add Regular Expression"), tr("Regular Expression:"), QLineEdit::Normal, "", &ok);
    else
        text = QInputDialog::getText(this, tr("Add New Word"), tr("Word:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        ui->listSearchWords->addItem(text);
        if (hasRegex)
            this->rule->regex.setPattern(text);
        else
            this->rule->keywords.append(text);
    }
}

void DialogHighlight::on_pushButtonEdit_clicked()
{
    bool ok;
    QListWidgetItem* item = ui->listSearchWords->selectedItems().first();
    QString text = QInputDialog::getText(this, tr("Edit Word"), tr("Word:"), QLineEdit::Normal, item->text(), &ok);
    if (ok && !text.isEmpty())
    {
        int pos = ui->listSearchWords->row(item);
        item->setText(text);
        if (hasRegex)
            this->rule->regex.setPattern(text);
        else
            this->rule->keywords[pos] = text;
    }
}

void DialogHighlight::on_pushButtonRemove_clicked()
{
    int res = QMessageBox::warning(this, tr("Remove Word"), tr("Are you sure to delete this word?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (res == QMessageBox::Yes)
    {
        QListWidgetItem* item = ui->listSearchWords->selectedItems().first();
        int pos = ui->listSearchWords->row(item);
        ui->listSearchWords->takeItem(pos);
        if (hasRegex)
            this->rule->regex.setPattern("");
        else
            this->rule->keywords.removeAt(pos);
    }
}

void DialogHighlight::on_btnPickerColor_clicked()
{
    QColor color(this->rule->color);
    QColorDialog dialog(color, this);
    dialog.show();
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        QColor result = dialog.selectedColor();
        DisplayColor(ui->btnPickerColor, result.name());
        this->rule->color = result.name();
    }
}

void DialogHighlight::on_textBoxName_textChanged()
{
    if (disconnect(ui->textBoxName, &QTextEdit::textChanged, this, &DialogHighlight::on_textBoxName_textChanged))
    {
        QTextCursor cursor = ui->textBoxName->textCursor();
        int posCursor = cursor.position();
        QString data = ui->textBoxName->toPlainText().remove("\n\r", Qt::CaseInsensitive).remove(QChar('\n'), Qt::CaseInsensitive);
        ui->textBoxName->setPlainText(data);
        cursor.setPosition(posCursor, QTextCursor::MoveAnchor);
        ui->textBoxName->setTextCursor(cursor);
        connect(ui->textBoxName, &QTextEdit::textChanged, this, &DialogHighlight::on_textBoxName_textChanged);
    }
}

KeywordHighlight DialogHighlight::showDialog(QString *nameRule, KeywordHighlight *rule)
{
    if (nameRule)
        ui->textBoxName->setText(*nameRule);
    if (rule)
    {
        this->rule = new KeywordHighlight(*rule);

        int comboIdx = -1;
        if ((comboIdx = ui->comboBoxFontWeight->findData(rule->weight)) != -1)
            ui->comboBoxFontWeight->setCurrentIndex(comboIdx);
        else
            ui->comboBoxFontWeight->setCurrentIndex(0);

        DisplayColor(ui->btnPickerColor, this->rule->color);

        if (this->rule->regex.pattern().count() > 0)
        {
            ui->listSearchWords->addItem(this->rule->regex.pattern());
            hasRegex = true;
        }
        else
        {
            ui->listSearchWords->addItems(this->rule->keywords);
        }
    }
    else
    {
        this->rule = new KeywordHighlight();
        DisplayColor(ui->btnPickerColor, QString("#000000"));
        QMessageBox box(
                    QMessageBox::Question,
                    tr("Regex or Keyword"),
                    tr("Si vuole creare una nuova regola con una Regular Expression oppure una lista di parole chiave?"));
        QPushButton *addRegex = box.addButton(tr("Reg. Exp."), QMessageBox::ActionRole);
        QPushButton *addKeyword = box.addButton(tr("Parole Chiave"), QMessageBox::ActionRole);

        while (true) {
            box.exec();
            if (addRegex == box.clickedButton())
            {
                hasRegex = true;
                break;
            }
            else if (addKeyword == box.clickedButton()) {
                break;
            }
        }
    }
    connect(ui->textBoxName, &QTextEdit::textChanged, this, &DialogHighlight::on_textBoxName_textChanged);

    do {
        this->show();
        this->exec();

        if (this->result() == QDialog::Accepted)
        {
            if (ui->textBoxName->toPlainText().count() <= 0)
                QMessageBox::warning(this, tr("Error"), tr("Name not entered!"));
            else
            {
                if (nameRule != ui->textBoxName->toPlainText())
                    *nameRule = ui->textBoxName->toPlainText();
                this->rule->weight = ui->comboBoxFontWeight->currentData().toInt();
                break;
            }
        }
        else
            break;
    }
    while (true);


    return *this->rule;
}
