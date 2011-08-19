#include "HighLighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    precompiler.setForeground(Qt::darkGray);
    precompiler.setFontWeight(QFont::Bold);
    QStringList precompilerPatterns;
    precompilerPatterns << "^#define\\b" << "^#undef\\b"
            << "^#if\\b" << "^#ifdef\\b" << "^#else\\b"
            << "^#elif\\b" << "^#endif\\b" << "^#error\\b"
            << "^#pragma\\b" << "^#extension\\b" << "^#version\\b"
            << "^#line\\b";
    foreach (const QString &pattern, precompilerPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = precompiler;
        highlightingRules.append(rule);
    }

    varTypes.setForeground(Qt::darkGreen);
    QStringList varTypePatterns;
    varTypePatterns << "\\bvoid\\b" << "\\bbool\\b" << "\\bint\\b"
            << "\\buint\\b" << "\\bfloat\\b" << "\\bdouble\\b" << "\\bstruct\\b"
            << "\\bvec2\\b" << "\\bvec3\\b" << "\\bvec4\\b"
            << "\\bdvec2\\b" << "\\bdvec3\\b" << "\\bdvec4\\b"
            << "\\bbvec2\\b" << "\\bbvec3\\b" << "\\bbvec4\\b"
            << "\\bivec2\\b" << "\\bivec3\\b" << "\\bivec4\\b"
            << "\\buvec2\\b" << "\\buvec3\\b" << "\\buvec4\\b"
            << "\\bmat2\\b" << "\\bmat3\\b" << "\\bmat4\\b"
            << "\\bmat2x2\\b" << "\\bmat2x3\\b" << "\\bmat2x4\\b"
            << "\\bmat3x2\\b" << "\\bmat3x3\\b" << "\\bmat3x4\\b"
            << "\\bmat4x2\\b" << "\\bmat4x3\\b" << "\\bmat4x4\\b"
            << "\\bdmat2\\b" << "\\bdmat3\\b" << "\\bdmat4\\b"
            << "\\bdmat2x2\\b" << "\\bdmat2x3\\b" << "\\bdmat2x4\\b"
            << "\\bdmat3x2\\b" << "\\bdmat3x3\\b" << "\\bdmat3x4\\b"
            << "\\bdmat4x2\\b" << "\\bdmat4x3\\b" << "\\bdmat4x4\\b"
            << "\\bsampler1D\\b" << "\\bsampler2D\\b" << "\\bsampler3D\\b"
            << "\\bsamplerCube\\b" << "\\bsampler2DRect\\b"
            << "\\bsampler2DShadow\\b" << "\\bsampler2DRectShadow\\b"
            << "\\bsampler1DArray\\b" << "\\bsampler2DArray\\b"
            << "\\bsampler1DArrayShadow\\b" << "\\bsampler2DArrayShadow\\b"
            << "\\bsamplerBuffer\\b" << "\\bsampler2DMS\\b" << "\\bsampler2DMSArray\\b"
            << "\\bsamplerCubeShadow\\b" << "\\bsamplerCubeArray\\b"
            << "\\bsamplerCubeArrayShadow\\b"
            << "\\bisampler1D\\b" << "\\bisampler2D\\b" << "\\bisampler3D\\b"
            << "\\bisamplerCube\\b" << "\\bisampler2DRect\\b"
            << "\\bisampler1DArray\\b" << "\\bisampler2DArray\\b"
            << "\\bisamplerBuffer\\b" << "\\bisampler2DMS\\b" << "\\bisampler2DMSArray\\b"
            << "\\bisamplerCubeArray\\b"
            << "\\busampler1D\\b" << "\\busampler2D\\b" << "\\busampler3D\\b"
            << "\\busamplerCube\\b" << "\\busampler2DRect\\b"
            << "\\busampler1DArray\\b" << "\\busampler2DArray\\b"
            << "\\busamplerBuffer\\b" << "\\busampler2DMS\\b" << "\\busampler2DMSArray\\b"
            << "\\busamplerCubeArray\\b";
    foreach (const QString &pattern, varTypePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = varTypes;
        highlightingRules.append(rule);
    }

    statement.setForeground(Qt::darkYellow);
    QStringList statementPatterns;
    statementPatterns << "\\bif\\b" << "\\belse\\b"
            << "\\bswitch\\b" << "\\bcase\\b" << "\\bdefault\\b"
            << "\\bfor\\b" << "\\bwhile\\b" << "\\bdo\\b"
            << "\\bdiscard\\b" << "\\breturn\\b" << "\\bbreak\\b" << "\\bcontinue\\b";
    foreach (const QString &pattern, statementPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = statement;
        highlightingRules.append(rule);
    }

    modifier.setForeground(Qt::magenta);
    QStringList modifierPatterns;
    modifierPatterns << "\\bconst\\b" << "\\bin\\b" << "\\bcentroid\\b"
            << "\\bsample\\b" << "\\bout\\b" << "\\battribute\\b" << "\\binout\\b"
            << "\\buniform\\b" << "\\bvarying\\b" << "\\bpatch\\b"
            << "\\bsmooth\\b" << "\\bflat\\b" << "\\bnoperspective\\b"
            << "\\bhighp\\b" << "\\bmediump\\b" << "\\blowp\\b"
            << "\\bprecisio\\b" << "\\binvariant\\b" ;
    foreach (const QString &pattern, modifierPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = modifier;
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
