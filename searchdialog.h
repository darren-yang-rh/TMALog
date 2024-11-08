// searchdialog.h

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QPlainTextEdit>
#define readlineno ;


class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    SearchDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~SearchDialog();

signals:
    void goToLine(int lineNumber);
    void sortLinesByLineNumber();
    void copyLine(const QString &text);
protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void searchClicked();
    void listItemClicked(QListWidgetItem *item);
    void selectallClicked();

private:
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *selectAllButton;
    QListWidget *resultList;
    QPlainTextEdit *plainTextEdit;
    void connectsignal();
    // Add the following private members for limiting resizing
    bool resizingAllowed = true;
    int originalWidth = 800;
    int originalHeight = 600;
};

#endif // SEARCHDIALOG_H
