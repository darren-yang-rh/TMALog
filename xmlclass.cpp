#include "xmlclass.h"
#include <QTextCharFormat>
void XmlSyntaxHighlighter::highlightBlock(const QString &text)
{

    QTextCharFormat elementFormat;
    elementFormat.setForeground(Qt::blue);

    QTextCharFormat attributeFormat;
    attributeFormat.setForeground(Qt::red);

    QRegularExpression elementPattern("<[\\w\\-]+");
    QRegularExpression endElementPattern("</[\\w\\-]+");

    QRegularExpressionMatchIterator elementMatches = elementPattern.globalMatch(text);
    while (elementMatches.hasNext())
    {
        QRegularExpressionMatch match = elementMatches.next();
        setFormat(match.capturedStart(), match.capturedLength(), elementFormat);
    }

    QRegularExpressionMatchIterator endElementMatches = endElementPattern.globalMatch(text);
    while (endElementMatches.hasNext())
    {
        QRegularExpressionMatch match = endElementMatches.next();
        setFormat(match.capturedStart(), match.capturedLength(), elementFormat);
    }

    QRegularExpression attributePattern("\\w+(?=\\s*=\\s*\"[^\"]*\")");
    QRegularExpressionMatchIterator attributeMatches = attributePattern.globalMatch(text);
    while (attributeMatches.hasNext())
    {
        QRegularExpressionMatch match = attributeMatches.next();
        setFormat(match.capturedStart(), match.capturedLength(), attributeFormat);
    }
}

