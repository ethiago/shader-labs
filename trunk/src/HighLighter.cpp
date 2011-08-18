#include "HighLighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bconst\\b" << "\\bdouble\\b" << "\\bint\\b"
        << "\\bstatic\\b" << "\\bstruct\\b" << "\\btypedef\\b"
        << "\\bunsigned\\b" << "\\bvoid\\b" << "\\bvec2\\b" << "\\bvec3\\b"
        << "\\bvec4\\b" << "\\bmat2\\b" << "\\bmat3\\b" << "\\bmat4\\b"
        << "\\bin\\b" << "\\bout\\b" << "\\binout\\b" << "\\buniform\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    globalVarFormat.setForeground(Qt::blue);
    globalVarFormat.setFontItalic(true);
    QStringList globalVarPatterns;
    globalVarPatterns << "\\bgl_Vertex\\b" << "\\bgl_Normal\\b" << "\\bgl_Position\\b"
        << "\\bgl_Color\\b" << "\\bgl_FrontColor\\b" << "\\bgl_BackColor\\b";
        // continuar
    foreach (const QString &pattern, globalVarPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = globalVarFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else
        {
            commentLength = endIndex - startIndex
            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
