#include "HighLighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    precompiler.setForeground(Qt::darkGray);
    precompiler.setFontWeight(QFont::Bold);
    QStringList precompilerPatterns;
    precompilerPatterns << "^#define\\b" << "^#undef\\b"
            << "^#if\\b" << "^#ifdef\\b" << "^#else\\b" << "^#elif\\b"
            << "^#endif\\b" << "^#error\\b" << "^#pragma\\b"
            << "^#extension\\b" << "^#version\\b" << "^#line\\b"
            << "\\b__LINE__\\b" << "\\b__FILE__\\b" << "\\b__VERSION__\\b";

    foreach (const QString &pattern, precompilerPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = precompiler;
        highlightingRules.append(rule);
    }

    varTypes.setForeground(Qt::darkCyan);
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
    statement.setFontWeight(QFont::Bold);
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

    modifier.setForeground(Qt::darkMagenta);
    QStringList modifierPatterns;
    modifierPatterns << "\\bconst\\b" << "\\bin\\b" << "\\bcentroid\\b"
            << "\\bsample\\b" << "\\bout\\b" << "\\battribute\\b" << "\\binout\\b"
            << "\\buniform\\b" << "\\bvarying\\b" << "\\bpatch\\b"
            << "\\bsmooth\\b" << "\\bflat\\b" << "\\bnoperspective\\b"
            << "\\bhighp\\b" << "\\bmediump\\b" << "\\blowp\\b"
            << "\\bprecision\\b" << "\\binvariant\\b" ;
    foreach (const QString &pattern, modifierPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = modifier;
        highlightingRules.append(rule);
    }

    globalConstant.setForeground(Qt::darkBlue);
    QStringList globalConstantPatterns;
    globalConstantPatterns << "\\bgl_MaxClipPlanes\\b"
            << "\\bgl_MaxCombinedTextureImageUnits\\b" << "\\bgl_MaxDrawBuffers\\b"
            << "\\bgl_MaxFragmentUniformComponents\\b" << "\\bgl_Max\\b"
            << "\\bgl_MaxLights\\b" << "\\bgl_MaxTextureCoords\\b"
            << "\\bgl_MaxTextureImageUnits\\b" << "\\bgl_MaxTextureUnits\\b"
            << "\\bgl_MaxVaryingFloats\\b" << "\\bgl_MaxVertexAttribs\\b"
            << "\\bgl_MaxVertexTextureImageUnits\\b"
            << "\\bgl_MaxVertexUniformComponents\\b";
    foreach (const QString &pattern, globalConstantPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = globalConstant;
        highlightingRules.append(rule);
    }


    globalVarFormat.setForeground(Qt::red);
    QStringList globalVarPatterns;
    globalVarPatterns
            << "\\bgl_ModelViewMatrix\\b" << "\\bgl_ModelViewProjectionMatrix\\b"
            << "\\bgl_ProjectionMatrix\\b" << "\\bgl_TextureMatrix\\b" << "\\bgl_NormalMatrix\\b"
            << "\\bgl_ModelViewMatrixInverse\\b" << "\\bgl_ModelViewProjectionMatrixInverse\\b"
            << "\\bgl_ProjectionMatrixInverse\\b" << "\\bgl_TextureMatrixInverse\\b"
            << "\\bgl_ModelViewMatrixTranspose\\b" << "\\bgl_ModelViewProjectionMatrixTranspose\\b"
            << "\\bgl_ProjectionMatrixTranspose\\b" << "\\bgl_TextureMatrixTranspose\\b"
            << "\\bgl_ModelViewMatrixInverseTranspose\\b"
            << "\\bgl_ModelViewProjectionMatrixInverseTranspose\\b"
            << "\\bgl_ProjectionMatrixInverseTranspose\\b"
            << "\\bgl_TextureMatrixInverseTranspose\\b"
            << "\\bgl_DepthRange\\b" << "\\bgl_Fog\\b" << "\\bgl_LightSource\\b"
            << "\\bgl_LightModel\\b"
            << "\\bgl_FrontLightModelProduct\\b" << "\\bgl_BackLightModelProduct\\b"
            << "\\bgl_FrontLightProduct\\b" << "\\bgl_BackLightProduct\\b"
            << "\\bgl_FrontMaterial\\b" << "\\bgl_BackMaterial\\b" << "\\bgl_Point\\b"
            << "\\bgl_NormalScale\\b" << "\\bgl_ClipPlane\\b" << "\\bgl_TextureEnvColor\\b"
            << "\\bgl_EyePlaneS\\b" << "\\bgl_EyePlaneT\\b"
            << "\\bgl_EyePlaneR\\b" << "\\bgl_EyePlaneQ\\b"
            << "\\bgl_ObjectPlaneS\\b" << "\\bgl_ObjectPlaneT\\b"
            << "\\bgl_ObjectPlaneR\\b" << "\\bgl_ObjectPlaneQ\\b"
            << "\\bgl_Vertex\\b" << "\\bgl_Normal\\b" << "\\bgl_Color\\b"
            << "\\bgl_SecondaryColor\\b" << "\\bgl_FogCoord\\b"
            << "\\bgl_FrontColor\\b" << "\\bgl_BackColor\\b"
            << "\\bgl_FrontSecondaryColor\\b" << "\\bgl_BackSecondaryColor\\b"
            << "\\bgl_TexCoord\\b" << "\\bgl_FogFragCoord\\b" << "\\bgl_Position\\b"
            << "\\bgl_PointSize\\b" << "\\bgl_ClipVertex\\b"
            << "\\bgl_FragColor\\b" << "\\bgl_FrontFacing\\b"
            << "\\bgl_FragData\\b" << "\\bgl_FragDepth\\b" <<
               "\\bgl_MultiTexCoord[0-5]\\b";
    foreach (const QString &pattern, globalVarPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = globalVarFormat;
        highlightingRules.append(rule);
    }

    globalFunctionFormat.setForeground(Qt::darkRed);
    QStringList globalFunctionsPatterns;
    globalFunctionsPatterns << "\\bsin\\b" << "\\bcos\\b" << "\\btan\\b"
            << "\\basin\\b" << "\\bacos\\b" << "\\batan\\b"
            << "\\bradians\\b" << "\\bdegrees\\b" << "\\bpow\\b"
            << "\\bexp\\b" << "\\blog\\b" << "\\bexp2\\b" << "\\blog2\\b"
            << "\\bsqrt\\b" << "\\binversesqrt\\b"
            << "\\babs\\b" << "\\bceil\\b" << "\\bclamp\\b" << "\\bfloor\\b"
            << "\\bfract\\b" << "\\bmax\\b" << "\\bmin\\b"
            << "\\bmix\\b" << "\\bmod\\b" << "\\bsign\\b"
            << "\\bsmoothstep\\b" << "\\bstep\\b"
            << "\\bftransform\\b" << "\\bcross\\b" << "\\bdistance\\b" << "\\bdot\\b"
            << "\\bfaceforward\\b" << "\\blength\\b"
            << "\\bnormalize\\b" << "\\breflect\\b"
            << "\\bdFdx\\b" << "\\bdFdy\\b" << "\\bfwidth\\b"
            << "\\matrixCompMult\\b"
            << "\\ball\\b" << "\\bany\\b" << "\\bequal\\b" << "\\bgreaterThan\\b"
            << "\\bgraterThenEqual\\b" << "\\blessThen\\b" << "\\blessThenEqual\\b"
            << "\\bnot\\b" << "\\bnotEqual\\b"
            << "\\btexture1D\\b" << "\\btexture1DProj\\b" << "\\btexture2D\\b" << "\\btexture2DProj\\b"
            << "\\btexture3D\\b" << "\\btexture3DProj\\b" << "\\btextureCube\\b"
            << "\\bshadow1D\\b" << "\\bshadow2D\\b" << "\\bshadow1DProj\\b" << "\\bshadow2DProj\\b"
            << "\\btexture1DLod\\b" << "\\btexture1DProjLod\\b"
            << "\\btexture2DLod\\b" << "\\btexture2DProjLod\\b"
            << "\\btexture3DProjLod\\b" << "\\btextureCubeLod\\b"
            << "\\bshadow1DLod\\b" << "\\bshadow2DLod\\b"
            << "\\bshadow1DProjLod\\b" << "\\bshadow2DProjLod\\b"
            << "\\bnoise1\\b" << "\\bnoise2\\b" << "\\bnoise3\\b" << "\\bnoise4\\b";
    foreach (const QString &pattern, globalFunctionsPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = globalFunctionFormat;
        highlightingRules.append(rule);
    }



    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

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
