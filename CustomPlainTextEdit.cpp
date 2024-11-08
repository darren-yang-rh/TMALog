#include "CustomPlainTextEdit.h"
#include "qregularexpression.h"
#include <QTextBlock>
#include <QTextCursor>
#include <QPainter>
#include <QPlainTextEdit>
#include <QColor>
#include <QDateTime>
#include <QScrollBar>

void CustomPlainTextEdit::mouseDoubleClickEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        QPlainTextEdit::mouseDoubleClickEvent(event);
        QTextCursor cursor = cursorForPosition(event->pos());
        QString selectedText = cursor.block().text().trimmed();
        QString formattedText;
#ifdef readlineno
        formattedText = selectedText;
#else

        static QRegularExpression fregex("^\\d+:\\+");

        if (fregex.match(selectedText).hasMatch())
        {
            formattedText = selectedText;
        }
        else
        {
            int lineNumber = cursor.blockNumber() + 1;
            QChar character = static_cast<QChar>(fileno);
            if (!filecolor[statusLabel2->text()].isNull())
                character = filecolor[statusLabel2->text()];

            //QString linePrefix = QString("%1:"+character+"     ").arg(lineNumber);

            //formattedText = QString("%1 %2").arg(linePrefix).arg(selectedText);
            formattedText = QString("%1 %2").arg(linePrefix,selectedText);

        }
#endif

        emit lineDoubleClicked(formattedText);
    }
}




void CustomPlainTextEdit::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    updateCursorLineBackground();
}

void CustomPlainTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
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
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_G)
    {
        emit goToLine2();
    }
    else
    {
        event->ignore();
    }
}

void CustomPlainTextEdit::mousePressEvent(QMouseEvent* event)
{
    QPlainTextEdit::mousePressEvent(event);
    updateCursorLineBackground();
}

/* Draw Cursor highlight*/

void CustomPlainTextEdit::updateCursorLineBackground()
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
void CustomPlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu(); // Get the default context menu
    menu->addAction(searchAction); // Add the custom action
    menu->addAction(selectAllAndSendAction);
    menu->addAction(syncAction);
    menu->exec(event->globalPos()); // Show the context menu at the global position
    delete menu;
}

void CustomPlainTextEdit::performSearch()
{
    // Implement your custom search functionality here
    //QMessageBox::information(this, "Search", "Performing custom search...");
    // Emit the custom signal when the search action is triggered
    emit searchTriggered();
}

void CustomPlainTextEdit::selectAllAndSend()
{

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);

    while (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        QString selectedText = cursor.selectedText().trimmed();
        if (!selectedText.isEmpty()) {
            emit selectSend(selectedText);
        }
        cursor.movePosition(QTextCursor::NextBlock);
    }
    emit sortLinesByLineNumber2();
}


void CustomPlainTextEdit::performsync()
{
    emit on_sync_triggered();
}
