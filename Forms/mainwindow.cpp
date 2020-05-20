#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Add Options button on Menu toolbar
    QAction* action = new QAction("Options");
    connect(action, &QAction::triggered, this, &MainWindow::on_actionMenuOptions_triggered);
    ui->menubar->addAction(action);

    //Add Event TextChanged on textEditor for highlight the code
    connect(ui->textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);

    //Add Event TextChanged on every textEditor for highlight the code
//    for (int i=0;i<ui->tabTextEditor->count();i++) {
//        QWidget* wdgt = ui->tabTextEditor->widget(i);
//        if (wdgt && strncmp(wdgt->metaObject()->className(), "QHBoxLayout", strlen("QHBoxLayout")) && wdgt->children().count() > 0) {
//            for (int j=0;j<wdgt->children().count();j++) {
//                if (wdgt->children().at(j) && strncmp(wdgt->children().at(j)->metaObject()->className(), "QTextEdit", strlen("QTextEdit")))
//                {
//                    QTextEdit* textEdit = (QTextEdit*)wdgt->children().at(j);
//                    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
//                }
//            }
//        }
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMenuExit_triggered()
{
    engine->Close();
    QApplication::exit();
}

void MainWindow::on_actionMenuOptions_triggered()
{
    OptionsDialog dialog(this);
    dialog.settings = globalSettings;
    dialog.show();
    dialog.exec();
}

void MainWindow::on_actionMenuRun_triggered()
{
    if (!this->engine)
        this->engine = new Engine();
//    if(this->ui->textEditor->toPlainText().size() > 0) {
//        if (this->engine->New()) {
//            QString codeString = this->ui->textEditor->toPlainText();
//            QByteArray code8bit = codeString.toLocal8Bit();
//            char *code = code8bit.data();
//            const char *result = this->engine->Run(code);
//            this->ui->textResults->append(result);
//            return;
//        }
//    }
//    this->ui->textResults->append("Error\n");
}
void MainWindow::on_actionMenuClear_triggered()
{
//    ui->textResults->setPlainText("");
}
void MainWindow::on_actionMenuNew_triggered()
{
//    ui->textEditor->setPlainText("");
}

void MainWindow::on_textEditor_textChanged()
{
    if (QString(this->sender()->metaObject()->className()) == "QTextEdit") {
        QTextEdit* textEditor = (QTextEdit*)this->sender();
        if (disconnect(textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged))
        {
            QTextCursor cursor = textEditor->textCursor();
            int posCursor = cursor.position();

            QString dataOriginal = textEditor->toPlainText();
            QString code = highlightCode(dataOriginal);

            textEditor->setHtml(code);
            cursor.setPosition(posCursor, QTextCursor::MoveAnchor);
            textEditor->setTextCursor(cursor);
            connect(textEditor, &QTextEdit::textChanged, this, &MainWindow::on_textEditor_textChanged);
        }
    }
}

QString MainWindow::highlightCode(QString code)
{
    if (!globalSettings) return code;
    code = code.replace("\t", "  ");
    map<QString, KeywordHighlight> highlights = globalSettings->fontSettings.highlightings;
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

    QString startHtml = QString("<pre style=\"color:KEYCOLOR;\">").replace("KEYCOLOR", colorDefault);
    QString endHtml = QString("</pre>");
    return code.insert(0, startHtml).prepend(endHtml);

}
