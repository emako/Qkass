#include <QtGui>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    this->initSyntax();
}

void Highlighter::initSyntax(void)
{
    isEmpty = true;
    keyword_format.setForeground(QColor((int)HighLighterKeyword::r, (int)HighLighterKeyword::g, (int)HighLighterKeyword::b));
    keyword_format.setFontWeight(QFont::Bold);
    section_line_format.setForeground(QColor((int)HighLighterSectionLine::r, (int)HighLighterSectionLine::g, (int)HighLighterSectionLine::b));
    section_line_format.setFontWeight(QFont::Bold);
    comment_line_format.setForeground(QColor((int)HighLighterCommentLine::r, (int)HighLighterCommentLine::g, (int)HighLighterCommentLine::b));
    format_line_format.setForeground(QColor((int)HighLighterFormatLine::r, (int)HighLighterFormatLine::g, (int)HighLighterFormatLine::b));
    format_line_format.setFontWeight(QFont::Bold);
}

void Highlighter::highlightBlock(const QString &text)
{
    for(const HighlightingRule &rule : highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}

void Highlighter::addSyntaxKeywords(const QStringList& keywords)
{
    HighlightingRule rule;
    for(QString keyword : keywords)
    {
        QString pattern = QString(ASS_TAG_KEYWORD_QSTR).arg(keyword);
        rule.pattern = QRegExp(pattern);
        rule.format = keyword_format;
        highlightingRules.append(rule);
    }
    this->completeSyntax();
}

void Highlighter::addSyntaxKeywords(const QStringList& keywords, const QTextCharFormat& format)
{
    HighlightingRule rule;
    for(QString keyword : keywords)
    {
        QString pattern = QString(ASS_TAG_KEYWORD_QSTR).arg(keyword);
        rule.pattern = QRegExp(pattern);
        rule.format = format;
        highlightingRules.append(rule);
    }
    this->completeSyntax();
}

void Highlighter::addSyntax(const QString& pattern, const QTextCharFormat& format)
{
    HighlightingRule rule;
    rule.pattern = QRegExp(pattern);
    rule.format = format;
    highlightingRules.append(rule);
    this->completeSyntax();
}

void Highlighter::completeSyntax(void)
{
    isEmpty = false;
}

void Highlighter::clearSyntax(void)
{
    highlightingRules.clear();
    isEmpty = true;
}

bool Highlighter::isHasRules(void)
{
    return isEmpty;
}
