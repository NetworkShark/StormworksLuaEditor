#include "qcodeedit.h"

QCodeEdit::QCodeEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &QCodeEdit::blockCountChanged, this, &QCodeEdit::updateLineNumberAreaWidth);
    connect(this, &QCodeEdit::updateRequest, this, &QCodeEdit::updateLineNumberArea);
    connect(this, &QCodeEdit::cursorPositionChanged, this, &QCodeEdit::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void QCodeEdit::LineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNum = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString num = QString::number(blockNum + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, num);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        blockNum++;
    }
}

int QCodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        digits++;
    }

    return 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void QCodeEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void QCodeEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QCodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSels;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection sel;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        sel.format.setBackground(lineColor);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = textCursor();
        extraSels.append(sel);
    }

    setExtraSelections(extraSels);
}

void QCodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}
