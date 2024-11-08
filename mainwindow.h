#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include "CustomPlainTextEdit.h"
#include "CustomPlainTextEdit2.h"
#define readlineno ;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Re_triggered();
    void on_actionOpen_triggered();
    void on_actionNAS_all_triggered();
    void on_actionRegister_triggered();
    void on_actionDefault_Parse_triggered();
    void onLineDoubleClicked(const QString &text);
    void on_actionabout_triggered();
    void on_actionSearch_triggered();
    void on_selectSend_triggered(const QString &text);
    void updateStatus();
    void on_actionNAS_only_triggered();
    void goToLine(int);
    void goToLine2();
    void copyLine(const QString &text);
    void sortLinesByLineNumber();
    void sortLinesByLineNumber2();
    void on_actionCell_info_triggered();
    void on_actionRSRP_triggered();
    void on_actionForw_command_triggered();
    void on_actionFrequency_triggered();
    void on_actionEraser_triggered();
    void on_actionSave_Note_triggered();
    void on_actionClose_triggered();
    void on_actionTRC_DU_CU_triggered();
    void on_actionEdit_XML_triggered();

    void on_actionClose_Current_triggered();

    void on_actionChartMouse_triggered();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_actionRRC_triggered();

    void on_actionOTA_triggered();

    void on_actionRRM_triggered();

    void on_actionRRC_Src_triggered();

    void on_actionINFO_triggered();

    void on_actionS1AP_triggered();
    void on_sync_triggered();
    void on_sync_triggered2();


    void on_actionGoto_Line_Ctrl_G_triggered();

    void on_actionSave_Note_text_triggered();

    void on_actionEMM_all_triggered();

private:
    QSet<QString> copiedLines;
    CustomPlainTextEdit* textEdit1;
    CustomPlainTextEdit* textEdit2;
    //QPlainTextEdit* textEdit3;
    CustomPlainTextEdit2* textEdit3;
    QStatusBar *statusBar;
    Ui::MainWindow *ui;
    QLabel *statusLabel;
    QLabel *statusLabel2;
    QString ver;
    void connectSignals();
    void RegularExecute(QString,CustomPlainTextEdit*,CustomPlainTextEdit*);
    void  applyHighlighting(QTextDocument*, const QString&, const QColor&);
    void applyStylesFromXml(CustomPlainTextEdit2*, const QString&);
    QString formatTimestamp(const QString &timestamp);
    void copiedLine_adjust(const QString &text);
    //std::unordered_map<QChar,QString> filecolor;
    std::unordered_map<QString,QChar> filecolor;
    int fileno=33; /*ASCII 42 represents file*/

};
#endif // MAINWINDOW_H
