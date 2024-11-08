#include "customplaintextedit2.h"
#include <QTextFormat>
#include <QTextBlock>
#include <QPainter>
#include <QScrollBar>

/*
void CustomPlainTextEdit2::setLineFileInfo(int linecount,const QString &filePath, int extraValue)
{
   // QTextCharFormat format;
    lineInfo[linecount]=qMakePair(filePath, extraValue);
}

QPair<QString, int> CustomPlainTextEdit2::getLineFileInfo(int lineNumber) const {
    if (lineNumber >= 0 && lineNumber < lineInfo.size()) {
        return lineInfo[lineNumber];
    }
    return QPair<QString, int>();
}*/
void CustomPlainTextEdit2::paintEvent(QPaintEvent *event)
{
   QPlainTextEdit::paintEvent(event);
 /*   //QPainter painter(viewport());
    QPainter painter(this);
    QTextBlock block = firstVisibleBlock();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();

/* Foreground color
    QStringList colorList = {
        "#000000", "#ff1493", "#7b68ee", "#6b8e23", "#4169e1","#0000ff","#006400", "#2f4f4f","#800000" , "#808000"
    };*/
/*  Background color
    QStringList colorList = {
        "#d3d3d3","#fafad2", "#90ee90","#e0ffff" , "#d3d3d3",
        "#ffb6c1", "#ffa07a", "#20b2aa", "#87cefa", "#778899"
    };


    while (block.isValid() && top <= event->rect().bottom()) {
        QString lineText = block.text();
        int firstColonIndex = lineText.indexOf(":");
        if (firstColonIndex != -1 && firstColonIndex + 1 < lineText.length()) {
            QChar asciiChar = lineText.at(firstColonIndex + 1);
            int asciiValue = static_cast<int>(asciiChar.toLatin1());

            int modValue = asciiValue % 10; // Calculate mod 10

            if (modValue >= 0 && modValue < colorList.size()) {
                QColor bgColor(colorList[modValue]);

                QTextCharFormat format = block.charFormat(); // Get current char format
                format.setBackground(bgColor); // Set background color
                //format.setForeground(bgColor); // Set background color
                format.setProperty(QTextFormat::FullWidthSelection, true);
                QTextCursor cursor(block);
                cursor.select(QTextCursor::BlockUnderCursor);
                cursor.setCharFormat(format); // Apply new format

                QTextLayout *layout = block.layout();
                if (layout) {
                    QTextLine textLine = layout->lineForTextPosition(firstColonIndex + 1);
                    if (textLine.isValid()) {
                        qreal x = textLine.cursorToX(firstColonIndex + 1);
                        qreal y = textLine.y();
                        painter.fillRect(x, y, textLine.cursorToX(lineText.length()), textLine.height(), bgColor);
                    }
                }
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    }
*/
}



void CustomPlainTextEdit2::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    updateCursorLineBackground();
}

void CustomPlainTextEdit2::keyPressEvent(QKeyEvent* event)
{
 /*   if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        QPlainTextEdit::keyPressEvent(event);
        updateCursorLineBackground();
    }
    else if (event->key() == Qt::Key_PageUp)
    {
        // Handle PageUp key
        verticalScrollBar()->setValue(verticalScrollBar()->value() - visibleRegion().boundingRect().height());
    }
    else if (event->key() == Qt::Key_PageDown)
    {
        // Handle PageDown key
        verticalScrollBar()->setValue(verticalScrollBar()->value() + visibleRegion().boundingRect().height());
    }
    else
    {
        event->ignore();
    }*/

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown ||
        event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ||
        event->key() == Qt::Key_Home || event->key() == Qt::Key_End)
    {
        QPlainTextEdit::keyPressEvent(event);
        //updateCursorLineBackground();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
        //event->ignore();
    }
}

void CustomPlainTextEdit2::mousePressEvent(QMouseEvent* event)
{
    QPlainTextEdit::mousePressEvent(event);
    updateCursorLineBackground();
}

/* Draw Cursor highlight*/

void CustomPlainTextEdit2::updateCursorLineBackground()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    // cusor position
    QTextCursor cursor = textCursor();
    int currentLineNumber = cursor.blockNumber();

    // clear previous slection
    setExtraSelections(extraSelections);

    // set cursor hughtlight color
    QTextEdit::ExtraSelection lineSelection;
    lineSelection.format.setBackground(QColorConstants::Svg::cyan);
    lineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineSelection.cursor = cursor;
    extraSelections.append(lineSelection);

    // update new highlight color
    setExtraSelections(extraSelections);

    // clear other non highlight color
    for (int i = 0; i < document()->blockCount(); ++i)
    {
        if (i != currentLineNumber)
        {
            QTextBlock block = document()->findBlockByLineNumber(i);
            block.clearLayout();
        }
    }
}
