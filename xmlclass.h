#ifndef XMLCLASS_H
#define XMLCLASS_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#define readlineno ;

class XmlSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    XmlSyntaxHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent)
    {
    }

protected:
    void highlightBlock(const QString &text) override;
};
#endif // XMLCLASS_H
