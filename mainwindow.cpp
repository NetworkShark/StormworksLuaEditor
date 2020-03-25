#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    engine->Close();
    QApplication::exit();
}
void MainWindow::on_actionMenuRun_triggered()
{
    if (!this->engine)
        this->engine = new Engine();
    if(this->ui->textEditor->toPlainText().size() > 0) {
        if (this->engine->New()) {
            QString codeString = this->ui->textEditor->toPlainText();
            QByteArray code8bit = codeString.toLocal8Bit();
            char *code = code8bit.data();
            const char *result = this->engine->Run(code);
            this->ui->textResults->append(result);
            return;
        }
    }
    this->ui->textResults->append("Error\n");
}
void MainWindow::on_actionMenuClear_triggered()
{
    ui->textResults->setPlainText("");
}
void MainWindow::on_actionMenuNew_triggered()
{
    ui->textEditor->setPlainText("");
}

void MainWindow::on_textEditor_textChanged()
{
    if (disconnect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged))
    {
        QTextCursor cursor = ui->textEditor->textCursor();
        int posCursor = cursor.position();

        QString dataOriginal = ui->textEditor->toPlainText();

        QString code = highlightCode(dataOriginal);

        ui->textEditor->setHtml(code);
        cursor.setPosition(posCursor, QTextCursor::MoveAnchor);
        ui->textEditor->setTextCursor(cursor);
        connect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
    }
}

QString MainWindow::highlightCode(QString code)
{
    if (!globalSettings) return code;
    map<QString, KeywordHighlight> highlights = globalSettings->highlightings;
    const char* startHighlightTag = "<font color=\"KEYCOLOR\">";
    QString endHighlightTag = "</font>";
    QRegExp regexpSplitCode("(--[^\\n]*|\\w+)");

    int pos = 0;
    QString color, colorDefault = "";
    while ((pos = regexpSplitCode.indexIn(code, pos)) != -1)
    {
        bool found = false;
        QString word = regexpSplitCode.cap(1);
        for (map<QString, KeywordHighlight>::iterator item=highlights.begin();item!=highlights.end();++item) {
            if (item->second.defaultColor) {
                if (colorDefault.size() == 0) colorDefault = item->second.color;
            } else if (item->second.keywords.contains(word)) {
                found = true;
            } else if (item->second.regex.exactMatch(word)) {
                found = true;
            }
            if (found) {
                color = item->second.color;
                break;
            }
        }
        if (found) {
            QString coloredStartTag = QString(startHighlightTag).replace("KEYCOLOR", color);
            code.insert(pos+regexpSplitCode.matchedLength(), endHighlightTag);
            code.insert(pos, coloredStartTag);
            pos += coloredStartTag.size() + endHighlightTag.size();
        }
        pos += regexpSplitCode.matchedLength();
    }

    QString startHtml = QString("<font color=\"KEYCOLOR\"><pre>").replace("KEYCOLOR", colorDefault);
    QString endHtml = QString("</pre></font>");
    return code.replace("\t", "    ").insert(0, startHtml).prepend(endHtml);

}

void MainWindow::on_actionMenuPalette_triggered()
{
    Settings tmpSetting = *globalSettings;
    PaletteSettingsDialog dialog(this, &tmpSetting);
    dialog.show();
    dialog.exec();
    if (tmpSetting != *globalSettings) {
        globalSettings = &tmpSetting;
        globalSettings->writeConfig();
        emit on_textEditor_textChanged();
    }
}







































