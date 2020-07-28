#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent, Settings *settings)
    : QSyntaxHighlighter(parent)
{
    if (!settings) return;

    HighlightingRule rule;
    KeywordHighlight *tmp;

    // Keywords Lua Lev. 1
    tmp = &settings->fontSettings.highlightings["Lua_L1"];
    keyword_Lua_L1_Format.setForeground(QColor(tmp->color));
    keyword_Lua_L1_Format.setFontWeight(QFont::Bold);
    for (int i=0;i<tmp->keywords.count();i++) {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(tmp->keywords[i]));
        rule.format = keyword_Lua_L1_Format;
        highlightingRules.append(rule);
    }
    // Keywords Lua Lev. 2
    tmp = &settings->fontSettings.highlightings["Lua_L2"];
    keyword_Lua_L2_Format.setForeground(QColor(tmp->color));
    keyword_Lua_L2_Format.setFontWeight(QFont::Bold);
    for (int i=0;i<tmp->keywords.count();i++) {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(tmp->keywords[i]));
        rule.format = keyword_Lua_L2_Format;
        highlightingRules.append(rule);
    }
    // Keywords Stormworks Lev. 1
    tmp = &settings->fontSettings.highlightings["Stormworks_L1"];
    keyword_Stormworks_L1_Format.setForeground(QColor(tmp->color));
    keyword_Stormworks_L1_Format.setFontWeight(QFont::Bold);
    for (int i=0;i<tmp->keywords.count();i++) {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(tmp->keywords[i]));
        rule.format = keyword_Stormworks_L1_Format;
        highlightingRules.append(rule);
    }
    // Keywords Stormworks Lev. 2
    tmp = &settings->fontSettings.highlightings["Stormworks_L2"];
    keyword_Stormworks_L2_Format.setForeground(QColor(tmp->color));
    keyword_Stormworks_L2_Format.setFontWeight(QFont::Bold);
    for (int i=0;i<tmp->keywords.count();i++) {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(tmp->keywords[i]));
        rule.format = keyword_Stormworks_L2_Format;
        highlightingRules.append(rule);
    }

    // Qqualcosa
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Stringhe
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // function name
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Commenti
    tmp = &settings->fontSettings.highlightings["Comments"];
    singleLineCommentFormat.setForeground(QColor(tmp->color));
    rule.pattern = QRegularExpression(QStringLiteral("--[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(tmp->color));
    commentStartExpression = QRegularExpression(QStringLiteral("--\\[{2}"));
    commentEndExpression = QRegularExpression(QStringLiteral("--]{2}"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

