#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionMenuRun_triggered()
{
    engine = Engine();
    if(ui->textEditor->toPlainText().size() > 0) {
        if (engine.New()) {
            QString codeString = ui->textEditor->toPlainText();
            QByteArray code8bit = codeString.toLocal8Bit();
            char *code = code8bit.data();
            const char *result = engine.Run(code);
            ui->textResults->append(result);
            return;
        }
    }
    ui->textResults->append("Error\n");
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
    const char* keyColor = "#F92672";

    if (disconnect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged))
    {
        QTextCursor cursor = ui->textEditor->textCursor();
        QString data = colorWord(ui->textEditor->toPlainText().replace("\n", "<br />"), keywordsList, keyColor);
        ui->textEditor->setHtml(data);
        ui->textEditor->setTextCursor(cursor);
        connect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
    }
}

QString MainWindow::colorWord(QString code, QString words, QString color)
{
    const char* masterString = "<font color=\"KEYCOLOR\">\\1</font>";
    QString replaceString =
            QString(masterString).replace("KEYCOLOR", color);
    QRegExp regex = QRegExp(words);
    code.replace(regex, replaceString);
    return code;
}



























