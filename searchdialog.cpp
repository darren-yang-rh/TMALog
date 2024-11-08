// searchdialog.cpp

#include "searchdialog.h"
#include <QVBoxLayout>
#include <QTextDocument>
#include <QStringList>
#include <QPlainTextEdit>
//#include <QSettings>
//#include <QCompleter>

SearchDialog::SearchDialog(QWidget *parent, QPlainTextEdit *textEdit)
   // : QDialog(parent), plainTextEdit(textEdit)
 : QDialog(parent), plainTextEdit(textEdit)
{

    // Set the initial size
    resize(800, 600);

    this->setWindowIcon(QIcon(":/img/find.png"));
    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("Search", this);
    selectAllButton = new QPushButton("Select All and Send (take for a while)", this);
    selectAllButton->setStyleSheet("color: red;");

    resultList = new QListWidget(this);
    resultList->setSelectionMode(QAbstractItemView::MultiSelection); // Set the selection mode        

    QPalette palette = resultList->palette();
    palette = resultList->palette();
    palette.setColor(QPalette::Base, QColorConstants::Svg::bisque);
    palette.setColor(QPalette::Text, Qt::darkRed);
    resultList->setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(searchLineEdit);
    layout->addWidget(searchButton);
    layout->addWidget(selectAllButton);
    layout->addWidget(resultList);

    //resize(2 * size());
    setLayout(layout);
    setModal(false);  // Allow cross Window editing and selecting
    connectsignal();

    //setFixedSize(originalWidth, originalHeight);
}

void SearchDialog::connectsignal()
{
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(resultList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));
    connect(selectAllButton,  SIGNAL(clicked()), this, SLOT(selectallClicked()));

}
void SearchDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    if (resizingAllowed) {
        QSize newSize = event->size();
        int maxWidth = originalWidth * 2.5;
        int maxHeight = originalHeight * 2.5;
        if (originalWidth == 0 || originalHeight == 0) {
            // Set the initial size if not set
            originalWidth = newSize.width();
            originalHeight = newSize.height();
        } else if (newSize.width() > maxWidth || newSize.height() > maxHeight) {
            // Limit the resizing within bounds
            QSize limitedSize = event->size().boundedTo(QSize(maxWidth, maxHeight));
            resizingAllowed = false;
            resize(limitedSize);
            resizingAllowed = true;
        }

    } else {
        originalWidth = event->size().width();
        originalHeight = event->size().height();
    }
}

SearchDialog::~SearchDialog()
{
}

void SearchDialog::searchClicked()
{
    resultList->clear();

    QString keyword = searchLineEdit->text();
    QString searchText = plainTextEdit->toPlainText();
    QStringList lines = searchText.split("\n");

    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].contains(keyword, Qt::CaseInsensitive))
        {
#ifdef readlineno
            resultList->addItem(lines[i]);

#else
            QChar  = static_cast<QChar>(fileno);
            if (!filecolor[statusLabel2->text()].isNull())
                 = filecolor[statusLabel2->text()];
            //QString line = QString::number(i + 1) + ":+     " + lines[i];
            QString line = QString::number(i + 1) + ":"++"     " + lines[i];
            resultList->addItem(line);
#endif
        }
    }
}

void SearchDialog::listItemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    int lineNumber = text.split(":").first().toInt();
    emit copyLine(item->text());
    emit goToLine(lineNumber);
    emit sortLinesByLineNumber();
}

void SearchDialog::selectallClicked()
{
    // Select all items in resultList
    resultList->selectAll();
    // Get a list of all selected items in resultList
    QList<QListWidgetItem*> selectedItems = resultList->selectedItems();

    // Iterate through the selected items and call listItemClicked() for each of them
    for (QListWidgetItem* item : selectedItems)
    {
        emit copyLine(item->text());
    }
    emit sortLinesByLineNumber();

}
