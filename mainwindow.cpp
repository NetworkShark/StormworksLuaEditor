#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    QRegExp regexStringValidator("(\".+\"|\'.+\')");
    QRegExp regexNumbersValidator("(true|false|\\d+)");
    QRegExp regexCommentsValidator("(--[^\\n]*)");
    const keywordHighlight keywordsHighlight[4] = {
        { keywordLuaHtmlColor_L1, "break|do|else|elseif|end|for|function|if|in|local|repeat|return|then|until|while|or|and|not|math|table|string" },
        { keywordLuaHtmlColor_L2, "tostring|next|tonumber|type|pairs|ipairs|abs|acos|asin|abs|atan|ceil|cos|deg|exp|floor|fmod|huge|max|maxinteger|min|mininteger|modf|pi|rad|random|randomseed|sin|sqrt|tan|tointeger|type|ult|concat|insert|move|pack|remove|sort|unpack|byte|char|dump|find|format|gmatch|gsub|len|lower|match|packsize|reverse|sub|upper" },
        { keywordStormworksColor_L1, "screen|map|input|output|property" },
        { keywordStormworksColor_L2, "getWidth|getHeight|setColor|drawClear|drawLine|drawCircle|drawCircleF|drawRect|drawRectF|drawTriangle|drawTriangleF|drawText|drawTextBox|drawMap|setMapColorOcean|setMapColorShallows|setMapColorLand|setMapColorGrass|setMapColorSand|setMapColorSnow|screenToMap|mapToScreen|getBool|getNumber|setBool|setNumber|getText" }
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

    QString startHtml = QString("<font color=\"KEYCOLOR\"><pre>").replace("KEYCOLOR", codeHtmlColor);
    QString endHtml = QString("</pre></font>");
    return code.replace("\t", "    ").insert(0, startHtml).prepend(endHtml);

}

void MainWindow::on_actionMenuPalette_triggered()
{
    std::map<QString, QString> colors;
    colors.insert(std::pair<QString, QString>(QString("Code"), QString(codeHtmlColor)));
    colors.insert(std::pair<QString, QString>(QString("Strings"), QString(stringsHtmlColor)));
    colors.insert(std::pair<QString, QString>(QString("Numbers"), QString(numbersHtmlColor)));
    colors.insert(std::pair<QString, QString>(QString("Comments"), QString(commentsHtmlColor)));
    colors.insert(std::pair<QString, QString>(QString("Lua_L1"), QString(keywordLuaHtmlColor_L1)));
    colors.insert(std::pair<QString, QString>(QString("Lua_L2"), QString(keywordLuaHtmlColor_L2)));
    colors.insert(std::pair<QString, QString>(QString("Stormworks_L1"), QString(keywordStormworksColor_L1)));
    colors.insert(std::pair<QString, QString>(QString("Stormworks_L2"), QString(keywordStormworksColor_L2)));
    PaletteSettingsDialog dialog(this, &colors);
    dialog.show();
    dialog.exec();
}







































