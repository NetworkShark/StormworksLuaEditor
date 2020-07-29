#ifndef QCODEEDIT_H
#define QCODEEDIT_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

class QCodeEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    QCodeEdit(QWidget *parent = nullptr);

    void LineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
};

#endif // QCODEEDIT_H

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

class LineNumberArea : public QWidget
{

public:
    LineNumberArea(QCodeEdit *editor = nullptr) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->LineNumberAreaPaintEvent(event);
    }
private:
    QCodeEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
