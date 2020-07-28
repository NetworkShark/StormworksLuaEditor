#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include "structs.h"

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, Settings *settings = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    Settings* settings;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keyword_Lua_L1_Format;
    QTextCharFormat keyword_Lua_L2_Format;
    QTextCharFormat keyword_Stormworks_L1_Format;
    QTextCharFormat keyword_Stormworks_L2_Format;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};
