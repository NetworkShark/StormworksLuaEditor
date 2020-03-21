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
    QRegExp regexStringValidator("(\".+\"|\'.+\')");
    QRegExp regexNumbersValidator("(true|false|\\d+)");
    QRegExp regexCommentsValidator("(--[^\\n]*)");
    const keywordHighlight keywordsHighlight[2] = {
        { keywordLuaHtmlColor, "break|do|else|elseif|end|for|function|if|in|local|repeat|return|then|until|while|or|and|not" }, // rosso -> keyword Lua Std
        { keywordStormworksColor, "timer|start|stop|print|printColor|tostring|unpack|next|tonumber|type|pairs|ipairs|math|abs|acos|asin|abs|atan|ceil|cos|deg|exp|floor|fmod|huge|max|maxinteger|min|mininteger|modf|pi|rad|random|randomseed|sin|sqrt|tan|tointeger|type|ult|table|concat|insert|move|pack|remove|sort|unpack|string|byte|char|dump|find|format|gmatch|gsub|len|lower|match|packsize|reverse|sub|upper|screen|getWidth|getHeight|setColor|drawClear|drawLine|drawCircle|drawCircleF|drawRect|drawRectF|drawTriangle|drawTriangleF|drawText|drawTextBox|drawMap|map|setMapColorOcean|setMapColorShallows|setMapColorLand|setMapColorGrass|setMapColorSand|setMapColorSnow|screenToMap|mapToScreen|input|getBool|getNumber|output|setBool|setNumber|property|getBool|getNumber|getText" } // azzurro -> keyword Lua Stormworks
    };

    const char* startHighlightTag = "<font color=\"KEYCOLOR\">";
    QString endHighlightTag = "</font>";
    QRegExp regexpSplitCode("(--[^\\n]*|\\w+)");

    int pos = 0;
    int sizeArrayHighlight = sizeof(keywordsHighlight) / sizeof(keywordsHighlight[0]);

    while ((pos = regexpSplitCode.indexIn(code, pos)) != -1) {
        bool found = false;
        QString color;
        QString word = regexpSplitCode.cap(1);
        if (regexCommentsValidator.exactMatch(word)) {
            color = QString(commentsHtmlColor);
            found = true;
        } else if (regexStringValidator.exactMatch(word)) {
            color = QString(stringsHtmlColor);
            found = true;
        } else if (regexNumbersValidator.exactMatch(word)) {
            color = QString(numbersHtmlColor);
            found = true;
        } else {
            for (int i=0;i<sizeArrayHighlight;i++) {
                if (keywordsHighlight[i].keywords.contains(word))
                {
                    color = keywordsHighlight[i].color;
                    found = true;
                    break;
                }
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


    return code.replace("\t", "    ").insert(0, "<pre>").prepend("</pre>");

}


























