#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "xmlclass.h"
#include "CustomPlainTextEdit.h"
#include "CustomPlainTextEdit2.h"
#include "searchdialog.h"
#include <QPushButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QFile>
#include <QInputDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextBlock>
#include <QMessageBox>
#include <QPalette>
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>
#include <QSplitter>
#include <QIcon>
#include <QCoreApplication>
#include <QProcess>
//#include <QRandomGenerator>



//Main window UI//

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Modify ver no here;
    ver="1.93+";
    this->setWindowIcon(QIcon(":/img/findoutline.png"));

    textEdit1 = new CustomPlainTextEdit(ui->centralwidget);
    textEdit2 = new CustomPlainTextEdit(ui->centralwidget);
    //textEdit3 = new QPlainTextEdit(ui->centralwidget);
    textEdit3 = new CustomPlainTextEdit2(ui->centralwidget);
    connectSignals();

    //textEdit 不縮行顯示
    textEdit1->setLineWrapMode(QPlainTextEdit::NoWrap);
    textEdit2->setLineWrapMode(QPlainTextEdit::NoWrap);
    textEdit3->setLineWrapMode(QPlainTextEdit::NoWrap);

    // Create top QSplitter
    QSplitter* topSplitter = new QSplitter(Qt::Vertical, this);
    topSplitter->addWidget(textEdit1); // Add textEdit1 to topSplitter

    // Create QSplitter for bottom portion (textEdit2 and textEdit3)
    QSplitter* bottomSplitter = new QSplitter(Qt::Horizontal, this);
    bottomSplitter->addWidget(textEdit2);
    bottomSplitter->addWidget(textEdit3);
    bottomSplitter->setStretchFactor(0, 1); // textEdit2 will take 1/2 of the space
    bottomSplitter->setStretchFactor(1, 1); // textEdit3 will take 1/2 of the space

    topSplitter->addWidget(bottomSplitter); // Add bottomSplitter to topSplitter
    topSplitter->setStretchFactor(0, 1); // textEdit1 will take 1/2 of the space
    topSplitter->setStretchFactor(1, 1); // bottomSplitter will take 1/2 of the space

    // Create QVBoxLayout to hold the top QSplitter
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topSplitter);

    // Create QWidget to hold the QVBoxLayout
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);

    // Set the QWidget as the central widget
    setCentralWidget(centralWidget);

    //ui->centralwidget->setLayout(ui->gridLayout);
    setWindowTitle("TMA Log Checker "+ver);

    //Src  Windows UI color scheme  textEdit1
    QPalette palette = textEdit1->palette();
    palette.setColor(QPalette::Base, QColorConstants::Svg::gainsboro);
    //palette.setColor(QPalette::Text,  QColorConstants::Svg::black);
    textEdit1->setPalette(palette);


    //Regular Expression Windows UI color scheme  textEdit2
    palette = textEdit2->palette();
    palette.setColor(QPalette::Base, QColorConstants::Svg::lavenderblush);
    palette.setColor(QPalette::Text, Qt::darkRed);
    textEdit2->setPalette(palette);

    //Out Analysis Windows UI color scheme  textEdit3
    palette = textEdit3->palette();
    palette.setColor(QPalette::Base, QColorConstants::Svg::lemonchiffon);
    palette.setColor(QPalette::Text, QColorConstants::Svg::darkslategrey);
    textEdit3->setPalette(palette);

    // 建立狀態列
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    // 建立右側的訊息標籤
    statusLabel = new QLabel(this);
    statusBar->addPermanentWidget(statusLabel);
    statusLabel2 = new QLabel(this);
    palette = statusLabel2->palette();
    palette.setColor(QPalette::WindowText, Qt::blue);
    statusLabel2->setPalette(palette);

    // 更新QLabel以显示更改
    //label->update();
    statusBar->addWidget(statusLabel2);

}

//MainWindow signal slot
void MainWindow::connectSignals()
{
    connect(textEdit1, &CustomPlainTextEdit::lineDoubleClicked, this, &MainWindow::onLineDoubleClicked);
    connect(textEdit2, &CustomPlainTextEdit::lineDoubleClicked, this, &MainWindow::onLineDoubleClicked);
    connect(textEdit1, &CustomPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatus);
    connect(textEdit1, &CustomPlainTextEdit::goToLine2, this, &MainWindow::goToLine2);
    connect(textEdit2, &CustomPlainTextEdit::goToLine2, this, &MainWindow::goToLine2);
    connect(textEdit2, &CustomPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatus);
    // Assuming textEdit1 is an instance of CustomPlainTextEdit in MainWindow
    connect(textEdit1, &CustomPlainTextEdit::searchTriggered, this, &MainWindow::on_actionSearch_triggered);
    connect(textEdit1, &CustomPlainTextEdit::selectSend, this, &MainWindow::on_selectSend_triggered);
    connect(textEdit2, &CustomPlainTextEdit::searchTriggered, this, &MainWindow::on_actionSearch_triggered);
    connect(textEdit2, &CustomPlainTextEdit::selectSend, this, &MainWindow::on_selectSend_triggered);
    connect(textEdit1, &CustomPlainTextEdit::on_sync_triggered, this, &MainWindow::on_sync_triggered);
    connect(textEdit2, &CustomPlainTextEdit::on_sync_triggered, this, &MainWindow::on_sync_triggered2);
    connect(textEdit1, SIGNAL(sortLinesByLineNumber()), this, SLOT(sortLinesByLineNumber()));
    connect(textEdit2, SIGNAL(sortLinesByLineNumber2()), this, SLOT(sortLinesByLineNumber2()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

//Regular expression code engine
//======================================================================================================
void MainWindow::RegularExecute(QString regex,CustomPlainTextEdit *textEdit_in,CustomPlainTextEdit *textEdit_out)
{

    QString text = textEdit_in->toPlainText();
    QStringList lines = text.split("\n");
    QString matches;
    int lineNumber = 1;
    QChar character = static_cast<QChar>(fileno);
    if (!filecolor[statusLabel2->text()].isNull())
        character = filecolor[statusLabel2->text()];

    for (const QString& line : lines) {
        //ㄥ QRegularExpression re(regex,QRegularExpression::DotMatchesEverythingOption);
        QRegularExpression re(regex,QRegularExpression::MultilineOption);
        QRegularExpressionMatchIterator matchIterator = re.globalMatch(line);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            //matches +=  QString::number(lineNumber) + ":+     " + match.captured(0) + "\n";
            matches +=  QString::number(lineNumber) + ":"+character+"     " + match.captured(0) + "\n";
            //matches += match.captured(0) + "\n";
        }
        lineNumber++;
        textEdit_out->setPlainText(matches);

    }
}
void MainWindow::on_action_Re_triggered()
{

    QString regex = QInputDialog::getText(this, "輸入Regular Expression", "請輸入正則表達式:");
    if (!regex.isEmpty())
        RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionOpen_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this, "選擇文檔", "", "文本文件 (*.log *.txt *.csv *.dat *.XML)");
    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
#ifdef readlineno
            QString fileContent;
            int lineNumber = 1;

            statusLabel2->setText(fileName);
            QChar character = static_cast<QChar>(fileno);
            if (!filecolor[statusLabel2->text()].isNull())
                character = filecolor[statusLabel2->text()];

            while (!in.atEnd()) {
                QString line = in.readLine();
                int lineNumberA = line.split(":").first().toInt();
                if (lineNumberA == lineNumber )
                {
                    fileContent += line + "\n";

                }
                else
                {
                    fileContent += QString::number(lineNumber) + ":"+character+"     " + line + "\n";
                }
                lineNumber++;
            }

            textEdit1->setPlainText(fileContent);
            //customFileHilighter.hashMap[fileName]=lineNumber;
            //QChar character = static_cast<QChar>(fileno);
            if (filecolor[fileName].isNull())
             {
                 filecolor[fileName]=character;
                 fileno++;
             }
            file.close();
#else
            static int lineNumber = 1;
            textEdit1->setPlainText(in.readAll());
            statusLabel2->setText(fileName);
            //customFileHilighter.hashMap[fileName]=lineNumber;
            file.close();
#endif
        }
    }

}


void MainWindow::on_actionNAS_all_triggered()
{
    QString regex = "(message c1.*|\\d{2}:\\d{2}:\\d{2}:\\d{3} PCO_RRC_.*:|\\d{2}:\\d{2}:\\d{2}:\\d{3} PCO_CFG_NAS_.*:|^nr[a-z]*[^\\s]*\\s*:\\s*\{|nr[^ ]+ : \{)";
    //QString regex = "(message c1.*|\d{2}:\d{2}:\d{2}:\d{3}PCO_RRC_.*|PCO_CFG_NAS_.*:|^nr[a-z]*[^\s]*\s*:\s*\{)";
    RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionRegister_triggered()
{
    //QString regex = "(\\d+):(\\d{2}):(\\d{2}):(\\d{3}) (.*)\\nnrRegistration";
    //QString regex = "(\\d+):(\\d{2}):(\\d{2}):(\\d{3}) (.*)\\n(nrRegistration(?:Request|Accept|Complete)\\s*:\\s*\{)";
    QString regex = "(nrRegistration(?:Request|Accept|Complete)\\s*:\\s*\{)";
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionDefault_Parse_triggered()
{
    QString regex = "(Registration Result:*$|NAS:.*$|RRC:.*$|Cell Id:.*$|Pdu Session Id:.*$|Data Network Name:.*$|IPv4 Address:.*$|MODE ACTIVE.*$)";
    RegularExecute(regex,textEdit1,textEdit2);
}



void MainWindow::on_actionCell_info_triggered()
{
    QString regex = "(\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ I: CMPI L2.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ I: CMPI MTE.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ I: CMPI L1.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ I: CMPI RRC.*$|Cause.*$|Cell Id:.*$)";
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionRSRP_triggered()
{

    QString regex = "(\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ 1001\\s*-\\s*.*&|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ 1000\\s*-\\s*.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ \\[DM-RS Port.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ MEAS-GROUP-STATS-.*$|\\(Radio Context: .*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ \\[SSB ID\\].*$)";
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionForw_command_triggered()
{
    QString regex = "(\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ SETP.*$|Cause.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ forw mte.*$)";
    RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionFrequency_triggered()
{
    QString regex = "(\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3} UE:ID 0.*$|\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}\\:\\d{3}\\ \\[SSB ID\\].*$|Cell Id:.*$)";
    RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionNAS_only_triggered()
{
    QString regex = "(message c1.*|PCO_CFG_NAS_.*:|PCO_RRC_.*|^nr[a-z]*[^\\s]*\\s*:\\s*\{)";
    RegularExecute(regex,textEdit1,textEdit2);
}




void MainWindow::on_actionTRC_DU_CU_triggered()
{
    //QString regex = "[0-9]{2}/[0-9]{2}/[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]+.*(?:CU:DU:UE_ID|DU:CU:UE_ID).*$";
    QString regex = "\\[\\d{2}/\\d{2}/\\d{4} \\d{2}:\\d{2}:\\d{2}\\.\\d{6}\\].*(?:CU:DU:UE_ID|DU:CU:UE_ID).*$";
    RegularExecute(regex,textEdit1,textEdit2);
}

//=======================================================================================


// Check core jump and linecopy buffer handling amd editText1/editText2/editText3 update
//======================================================================================

void MainWindow::onLineDoubleClicked(const QString &text)
{
    if (!copiedLines.contains(text))
    {
        copiedLine_adjust(text);
        int lineNumber = text.split(":").first().toInt();
        goToLine(lineNumber);
        sortLinesByLineNumber();

    }
}
void MainWindow::copiedLine_adjust(const QString &text)
{
    // New regular expression to match time format
    static QRegularExpression timeRegex1("\\[(\\d{2}/\\d{2}/\\d{4}) (\\d{2}:\\d{2}:\\d{2}\\.\\d{6})\\]");
    static QRegularExpression timeRegex2("\\[(\\d{1,2}-\\d{1,2}-\\d{4}) (\\d{2}:\\d{2}:\\d{2}\\.\\d{6})\\]");

    QString selectedText = text.trimmed();
    QRegularExpressionMatch match1 = timeRegex1.match(selectedText);
    QRegularExpressionMatch match2 = timeRegex2.match(selectedText);

    if (match1.hasMatch())
    {
        //QString datePart = match1.captured(1);
        QString timePart = match1.captured(2);

        QString formattedTime = formatTimestamp(timePart);

        QString formattedText = selectedText;
        formattedText.replace(match1.captured(2), formattedTime); // Replace the original time with formatted time
        copiedLines.insert(formattedText);
    }
    else if (match2.hasMatch())
    {
        //QString datePart = match2.captured(1);
        QString timePart = match2.captured(2);

        QString formattedTime = formatTimestamp(timePart);

        QString formattedText = selectedText;
        formattedText.replace(match2.captured(2), formattedTime); // Replace the original time with formatted time
        copiedLines.insert(formattedText);
    }
    else
    {
        copiedLines.insert(text);
    }
}

QString MainWindow::formatTimestamp(const QString &timePart)
{
    static QRegularExpression timeRegex("(\\d{2}:\\d{2}:\\d{2}\\.\\d{6})");
    QRegularExpressionMatch match = timeRegex.match(timePart);

    if (match.hasMatch())
    {
        QString formattedMilliseconds = match.captured(1).mid(9, 3); // Extract first three milliseconds

        QString formattedTime = QString("%1:%2:%3:%4")
                                    .arg(match.captured(1).left(2), 2, '0')      // Hours
                                    .arg(match.captured(1).mid(3, 2), 2, '0')   // Minutes
                                    .arg(match.captured(1).mid(6, 2), 2, '0')   // Seconds
                                    .arg(formattedMilliseconds);                // First three milliseconds

        return formattedTime;
    }

    return QString(); // Return empty string if the format is unexpected
}
//Src windows TextLine go to cursor
void MainWindow::goToLine(int lineNumber)
{
    QTextCursor cursor = textEdit1->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNumber - 1);
    textEdit1->setTextCursor(cursor);
    textEdit1->ensureCursorVisible();
    textEdit1->updateCursorLineBackground();
}


void MainWindow::goToLine2()
{
    bool ok;
    int lineNumber = QInputDialog::getInt(this, tr("Go to Line"), tr("Line number:"), 1, 1, textEdit1->blockCount(), 1, &ok);
    if (ok)
    {
        QTextCursor cursor = textEdit1->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber - 1);
        textEdit1->setTextCursor(cursor);
        textEdit1->ensureCursorVisible();
        textEdit1->updateCursorLineBackground();
    }
}
void MainWindow::copyLine(const QString &text)
{
    if (!copiedLines.contains(text))
    {
        copiedLine_adjust(text);
    }
}


void MainWindow::sortLinesByLineNumber()
{
    //Sort Logic  1. Timestamp -- if line data has time format , it will be sort according to time
    //            2. if there is no Timestamp , sort it by lineno , but due to Timestamp is forst order , so lineno may be not correct order ,it should be acceptable
    textEdit3->clear();
    QList<QString> sortedLines = copiedLines.values();
    std::sort(sortedLines.begin(), sortedLines.end(), [](const QString &a, const QString &b) {
        // Check if the lines contain time format "dd/MM/yy HH:mm:ss:zzz"
        //static QRegularExpression timeRegex("\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}:\\d{3}");
        static QRegularExpression timeRegex("\\d{2}:\\d{2}:\\d{2}:\\d{3}");
        bool hasTimeFormatA = a.contains(timeRegex);
        bool hasTimeFormatB = b.contains(timeRegex);
        int lineNumberA = a.split(":").first().toInt();
        int lineNumberB = b.split(":").first().toInt();
        if ((hasTimeFormatA && hasTimeFormatB) & (lineNumberA != lineNumberB))
        {
            // If both lines have time format, extract time and compare
            QString timeStrA = timeRegex.match(a).captured(0);
            QString timeStrB = timeRegex.match(b).captured(0);
            if (timeStrA == timeStrB)
                return lineNumberA < lineNumberB;
            return timeStrA < timeStrB;
        }
        else if ((hasTimeFormatA) and (lineNumberA <= lineNumberB))
        {
            // If only line A has time format, put it before line B
              return true;
        }
         else if ((hasTimeFormatB) and (lineNumberA >= lineNumberB))
        {
            // If only line B has time format, put it before line A
            return false;
        }
        else
        {
            // If both lines don't have time format, compare them by line numbers
            return lineNumberA <= lineNumberB;
        }
    });


    //Out Analysis Windows UI color scheme  textEdit3

    for (const auto &line : sortedLines)
    {
        textEdit3->appendPlainText(line);       
    }
    /*  // Bellow is code of  "sort by lineno ..."
     textEdit3->clear();
     QList<QString> sortedLines = copiedLines.values();
     std::sort(sortedLines.begin(), sortedLines.end(), [](const QString &a, const QString &b) {
         int lineNumberA = a.split(":").first().toInt();
         int lineNumberB = b.split(":").first().toInt();
         return lineNumberA < lineNumberB;
     });
     for (const auto &line : sortedLines)
     {
        textEdit3->appendPlainText(line);
     }
*/
 applyStylesFromXml(textEdit3,"styles.xml");
}


void MainWindow::sortLinesByLineNumber2()
{
 //Sort Logic  1. Timestamp -- if line data has time format , it will be sort according to time
 //            2. if there is no Timestamp , sort it by lineno , but due to Timestamp is forst order , so lineno may be not correct order ,it should be acceptable
 textEdit3->clear();
 QList<QString> sortedLines = copiedLines.values();
 std::sort(sortedLines.begin(), sortedLines.end(), [](const QString &a, const QString &b) {
         // Check if the lines contain time format "dd/MM/yy HH:mm:ss:zzz"
     //static QRegularExpression timeRegex("\\d{2}/\\d{2}/\\d{2} \\d{2}:\\d{2}:\\d{2}:\\d{3}");
     static QRegularExpression timeRegex("\\d{2}:\\d{2}:\\d{2}:\\d{3}");
     bool hasTimeFormatA = a.contains(timeRegex);
     bool hasTimeFormatB = b.contains(timeRegex);
     int lineNumberA = a.split(":").first().toInt();
     int lineNumberB = b.split(":").first().toInt();
     if (hasTimeFormatA && hasTimeFormatB)
         {
         // If both lines have time format, extract time and compare
         QString timeStrA = timeRegex.match(a).captured(0);
         QString timeStrB = timeRegex.match(b).captured(0);
         if (timeStrA == timeStrB)
             return lineNumberA < lineNumberB;
         return timeStrA < timeStrB;
         }
      else
        return lineNumberA < lineNumberB;
 });

 //Out Analysis Windows UI color scheme  textEdit3
 for (const auto &line : sortedLines)
 {
        textEdit3->appendPlainText(line);
 }
 applyStylesFromXml(textEdit3,"styles.xml");
}
//================================================================================================

//Other Menu Item=================================================================================
void MainWindow::on_actionSearch_triggered()
{
    SearchDialog *searchDialog = new SearchDialog(this, textEdit1);
    connect(searchDialog, SIGNAL(goToLine(int)), this, SLOT(goToLine(int)));
    connect(searchDialog, SIGNAL(copyLine(QString)), this, SLOT(copyLine(QString)));
    connect(searchDialog, SIGNAL(sortLinesByLineNumber()), this, SLOT(sortLinesByLineNumber()));
    searchDialog->show();
}


void MainWindow::on_selectSend_triggered(const QString &text)
{
    //textEdit3->appendPlainText(textEdit2->toPlainText());
    //textEdit3->appendPlainText(text);
    if (!copiedLines.contains(text))
    {
        copiedLine_adjust(text);
    }

}
void MainWindow::on_sync_triggered()
{
    QTextCursor cursor1 = textEdit1->textCursor();
    QTextCursor cursor2 = textEdit2->textCursor();
    QString lineText = cursor1.block().text();
    QString lineText2 = cursor2.block().text();
    int lineNumber1 = lineText.split(":").first().toInt();
    int lineNumber2 = lineText2.split(":").first().toInt();
    if (lineNumber2 <= lineNumber1)
    {
        cursor1.movePosition(QTextCursor::Start);
        cursor1.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor,lineNumber2-1);
        textEdit1->setTextCursor(cursor1);
        textEdit1->ensureCursorVisible();
        textEdit1->updateCursorLineBackground();
     }
     else
    {
        cursor2.movePosition(QTextCursor::Start);
        cursor2.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        textEdit2->setTextCursor(cursor2);
        textEdit2->ensureCursorVisible();
        textEdit2->updateCursorLineBackground();
    }

}
void MainWindow::on_sync_triggered2()
{

    QTextCursor cursor1 = textEdit1->textCursor();
    QTextCursor cursor2 = textEdit2->textCursor();
    QString lineText = cursor2.block().text();
    int lineNumber = lineText.split(":").first().toInt();
    cursor1.movePosition(QTextCursor::Start);
    cursor1.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNumber - 1);
    textEdit1->setTextCursor(cursor1);
    textEdit1->ensureCursorVisible();
    textEdit1->updateCursorLineBackground();
}

void MainWindow::on_actionabout_triggered()
{
    QMessageBox messageBox;
    messageBox.setWindowIcon(QIcon(":/img/find.png"));
    messageBox.setText("TMA Log Checker "+ ver +"\n By Darren Yang");
    messageBox.setWindowTitle("About this Tool");
    QPixmap customIcon(":/img/find.png");
    messageBox.setIconPixmap(customIcon.scaledToWidth(64));
    messageBox.exec();
    //QMessageBox::information(this, "About Tool", "TMA Log Checker "+ ver +"\n By Darren Yang");
}


void MainWindow::updateStatus() {
    // 取得游標的位置
    QTextCursor cursor = textEdit1->textCursor();
    int line = cursor.blockNumber() + 1;  // 行數是從 1 開始
    int column = cursor.columnNumber() + 1;  // 列數是從 1 開始

    // 更新狀態列的文字
    statusLabel->setText(QString("col: %1 row: %2").arg(line).arg(column));
}
void MainWindow::on_actionEraser_triggered()
{
    copiedLines.clear();
    textEdit3->clear();
}


void MainWindow::on_actionSave_Note_triggered()
{
    // 获取文本编辑器的内容
   //QString text = textEdit3->toPlainText();

    // 获取文本编辑器的文档对象
    QTextDocument *document = textEdit3->document();

    // 将文档转换为HTML格式
    QString htmlContent = document->toHtml();

    // 弹出文件对话框
    QString fileName = QFileDialog::getSaveFileName(this, "Save as HTML", QString(), "HTML Files (*.html)");

    if (!fileName.isEmpty()) {
        // 尝试打开文件并写入HTML内容
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << htmlContent;
            file.close();
            QMessageBox::information(this, "Success", "HTML saved successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to save HTML!");
        }
    }
}


void MainWindow::on_actionClose_triggered()
{
    // Close the main window to quit the application
    close();
}

//================================================================================================
//future XML Handling ============================================================================

/*void MainWindow::applyHighlighting(QTextDocument* document, const QString& pattern, const QColor& color) {
    QStringList colorList = {
        "#ffe4b5", "#97ffff", "#7fffd4", "#e6e6fa", "#ffc1c1",
        "#98fb98", "#e9967a", "#cdb79e", "#cd919e", "#ffaeb9"
    };

    QTextCursor cursor(document);
    QTextCharFormat defaultFormat;
    QTextCharFormat highlightFormat;
    highlightFormat.setForeground(color);

    QString text = document->toPlainText();
    QStringList lines = text.split('\n');

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];
        int colonIndex = line.indexOf(':');
        if (colonIndex >= 0 && colonIndex < line.length() - 1) {  // 确保 ':' 不是最后一个字符
            QChar specialChar = line[colonIndex + 1];
            int asciiValue = specialChar.toLatin1();
            if (asciiValue != 33)
            {
              int modValue = asciiValue % 10;
              QColor backgroundColor(colorList[modValue]);
              defaultFormat.setBackground(backgroundColor);

            // 设置当前行的背景色
              cursor.movePosition(QTextCursor::Start);
              cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, i);
              cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
              cursor.mergeCharFormat(defaultFormat);


            }
        }
    }

    // 重新遍历文档以应用正则表达式匹配的前景色
    QRegularExpression regex(pattern);
    QRegularExpressionMatchIterator iterator = regex.globalMatch(text);

    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        cursor.setPosition(match.capturedStart());
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, match.capturedLength());
        cursor.mergeCharFormat(highlightFormat);
    }
}*/

void MainWindow::applyHighlighting(QTextDocument* document, const QString& pattern, const QColor& color) {
    QStringList colorList = {
        "#ffe4b5", "#97ffff", "#7fffd4", "#e6e6fa", "#ffc1c1",
        "#98fb98", "#e9967a", "#cdb79e", "#cd919e", "#ffaeb9"
    };

    QTextCursor cursor(document);
    QTextCharFormat highlightFormat;
    highlightFormat.setForeground(color);

    QString text = document->toPlainText();
    QStringList lines = text.split('\n');

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];
        int colonIndex = line.indexOf(':');
        if (colonIndex >= 0 && colonIndex < line.length() - 1) {  // 確保 ':' 不是最後一個字符
            QChar specialChar = line[colonIndex + 1];
            int asciiValue = specialChar.toLatin1();
            if (asciiValue != 33) {
                 int modValue = asciiValue % 10;
                 QColor backgroundColor(colorList[modValue]);

                 // 設置當前行的背景色
                 cursor.movePosition(QTextCursor::Start);
                 cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, i);
                 cursor.movePosition(QTextCursor::StartOfLine);

                 QTextBlockFormat blockFormat;
                 blockFormat.setBackground(backgroundColor);
                 cursor.mergeBlockFormat(blockFormat);
            }
        }
    }

    // 重新遍歷文檔以應用正則表達式匹配的前景色
    QRegularExpression regex(pattern);
    QRegularExpressionMatchIterator iterator = regex.globalMatch(text);

    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        cursor.setPosition(match.capturedStart());
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, match.capturedLength());
        cursor.mergeCharFormat(highlightFormat);
    }
}



void MainWindow::applyStylesFromXml(CustomPlainTextEdit2* textEdit, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  //20240514 modify to hardcode to instard off xml
    {  /* if no XML file*/
        QString pattern = "(\\d{2}:\\d{2}:\\d{2}:\\d{3}|\\d{2}/\\d{2}/\\d{4}\\d{2}:\\d{2}:\\d{2}\\.\\d{6}|Error|Failure|ERROR|FAILURE|FAILED)";
        applyHighlighting(textEdit3->document(), pattern, Qt::red);
        pattern = "(_STATE:.*$|CMPI.*|message c1 :.*:|REGISTERED.*)";
        applyHighlighting(textEdit3->document(), pattern, Qt::magenta);
        pattern = "(PCO_.+|forw mte.*|SETP.*)"; // 11:31:17:790 格式的時間
        applyHighlighting(textEdit3->document(), pattern, Qt::blue);
        qDebug() << "Failed to open XML file";
        return;
    }

    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        if (reader.readNext() == QXmlStreamReader::StartElement && reader.name() == QString("pattern"))
        {
            QString regexStr;
            QColor color;

            // Read the pattern elements
            while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QString("pattern")))
            {
                if (reader.tokenType() == QXmlStreamReader::StartElement) {
                    if (reader.name() == QString("regex")) {
                        regexStr = reader.readElementText();
                    } else if (reader.name() == QString("color")) {
                        reader.readNext(); // Move to the color value
                        color = QColor(reader.text().toString());
                        //qDebug()<<reader.readElementText();
                    }
                }                                
              reader.readNext();
            }
            if (!regexStr.isEmpty() && color.isValid()) {
                applyHighlighting(textEdit->document(), regexStr, color);
            }
        }
    }

    file.close();
}

//================================================================================================


void MainWindow::on_actionEdit_XML_triggered()
{
   /* QString filePath = QCoreApplication::applicationDirPath() + "/styles.xml";

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString xmlContent = in.readAll();
        file.close();

        textEdit3->setPlainText(xmlContent);

        // Apply syntax highlighting
        XmlSyntaxHighlighter *highlighter = new XmlSyntaxHighlighter(textEdit3->document());
    }
    else
    {
        // Opening styles.xml failed, try opening using file dialog
        filePath = QFileDialog::getOpenFileName(this, "Open Config XML File", "", "XML Files (*.xml)");
        if (!filePath.isEmpty())
        {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                QString xmlContent = in.readAll();
                file.close();

                textEdit3->setPlainText(xmlContent);

                // Apply syntax highlighting
                XmlSyntaxHighlighter *highlighter = new XmlSyntaxHighlighter(textEdit3->document());
            }
            else
            {
                QMessageBox::warning(this, "Error", "Failed to open the XML file.");
            }
        }
    }*/
}



void MainWindow::on_actionClose_Current_triggered()
{
    textEdit1->clear();
    textEdit2->clear();
}


void MainWindow::on_actionChartMouse_triggered()
{
/*    QProcess process;
    QString exePath = "chartmouse.exe";
    QStringList arguments; // 根據需要添加命令列參數

    process.start(exePath, arguments);
    process.waitForFinished();

    int exitCode = process.exitCode();
    qDebug() << "C#應用程式退出碼：" << exitCode;*/
    QProcess *process = new QProcess(this);
    QString exePath = "chartmouse.exe";
    QStringList arguments; // 根據需要添加命令列參數

    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));

    process->start(exePath, arguments);
}
void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "C#應用程式退出碼：" << exitCode;
    // 可以在這裡處理結束後的工作
}


void MainWindow::on_actionRRC_triggered()
{
    //QString regex = "[0-9]{2}/[0-9]{2}/[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]+.*(?:CU:DU:UE_ID|DU:CU:UE_ID).*$";
    QString regex = ".*\\b(RRM|RRC)\\b.*";
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionOTA_triggered()
{
    //QString regex = "[0-9]{2}/[0-9]{2}/[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]+.*(?:CU:DU:UE_ID|DU:CU:UE_ID).*$";
    QString regex(".*\\[OTA\\].*");
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionRRM_triggered()
{
    QString regex(".*:RRM.*");
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionRRC_Src_triggered()
{
    QString regex(".*\\b(CQI|RSRP)\\b.*");
    RegularExecute(regex,textEdit1,textEdit2);
}


void MainWindow::on_actionINFO_triggered()
{
    QString regex(".*\\bINFO:CRNTI\\b.*");
    RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionS1AP_triggered()
{
    QString regex(".*:\\[S1AP\\].*");
    RegularExecute(regex,textEdit1,textEdit2);

}


void MainWindow::on_actionGoto_Line_Ctrl_G_triggered()
{
     goToLine2();
}


void MainWindow::on_actionSave_Note_text_triggered()
{
     // 获取文本编辑器的内容
     QString text = textEdit3->toPlainText();

     // 弹出文件对话框
     QString fileName = QFileDialog::getSaveFileName(this, "Save as Text", QString(), "Text Files (*.txt)");

     if (!fileName.isEmpty()) {
        // 尝试打开文件并写入纯文本内容
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << text;
            file.close();
            QMessageBox::information(this, "Success", "Text saved successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to save text!");
        }
     }
}


void MainWindow::on_actionEMM_all_triggered()
{
     //QString regex(".*(?:EMM_Message|pdn|attach|detach|eventId).*");
     //QString regex("(?:\\bEMM_Message\\b|pdn|attach|detach|eventid).*");
     //QString regex("\\b(?:\\w*EMM_Message|\\w*EMM|pdn|attach|detach|eventId)\\b.*");
     QString regex("\\b(?:\\w*EMM_Message|\\w*EMM|eventId|pdn.*|attach.*|detach.*)\\b.*");
     RegularExecute(regex,textEdit1,textEdit2);
}

