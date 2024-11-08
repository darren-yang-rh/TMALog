#ifndef CUSTOMAINPLAINTEXTEDIT2_H
#define CUSTOMAINPLAINTEXTEDIT2_H

#include <QPlainTextEdit>
#include <QVector>
#include <QPair>

class CustomPlainTextEdit2 : public QPlainTextEdit
{
public:
    CustomPlainTextEdit2(QWidget *parent = nullptr) : QPlainTextEdit(parent)
    {
                    setReadOnly(false);
    };
    /*QPair<QString, int> getLineFileInfo(int) const;
    void setLineFileInfo(int linecount,const QString &filePath, int extraValue);*/

    QColor getBackgroundColor() const { return m_bgColor; }
    void updateCursorLineBackground();
private:
    QColor m_bgColor;
    QVector<QPair<QString, int>> lineInfo;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // CUSTOMAINPLAINTEXTEDIT2_H
