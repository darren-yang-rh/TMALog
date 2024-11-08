#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H
#include <QPlainTextEdit>
#include <QPalette>
#define readlineno ;
#include <QContextMenuEvent>
#include <QAction>
#include <QMessageBox>
#include <QMenu>


class CustomPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    CustomPlainTextEdit(QWidget *parent = nullptr) : QPlainTextEdit(parent)
    {
        setReadOnly(false);
        updateCursorLineBackground();
        // Create a custom action for "Search"
        searchAction = new QAction("Search", this);
        searchAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S)); // Use | instead of +
        connect(searchAction, &QAction::triggered, this, &CustomPlainTextEdit::performSearch);

        selectAllAndSendAction = new QAction(tr("Select All And Send"), this);
        connect(selectAllAndSendAction, &QAction::triggered, this, &CustomPlainTextEdit::selectAllAndSend);

        syncAction = new QAction(tr("Sync"), this);
        connect(syncAction, &QAction::triggered, this, &CustomPlainTextEdit::performsync);

        // Add the custom action to the context menu
        addAction(searchAction);
        addAction(syncAction);
        addAction(selectAllAndSendAction);
    };
    void updateCursorLineBackground();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
signals:
    void lineDoubleClicked(const QString &text);
    void goToLine2();
    void searchTriggered();// Custom signal for search action
    void selectSend(const QString &text);// Custom signal for search action
    void sortLinesByLineNumber();
    void sortLinesByLineNumber2();
    void on_sync_triggered();
private:
    QAction *searchAction;
    QAction *selectAllAndSendAction;
    QAction *syncAction;

private slots:
    void performsync();
    void performSearch();
    void selectAllAndSend();

};
#endif // CUSTOMPLAINTEXTEDIT_H
