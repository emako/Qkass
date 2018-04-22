#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include "tag.h"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
    void initSyntax(void);
    bool isHasRules(void);
    void completeSyntax(void);
    void clearSyntax(void);

    QTextCharFormat text_format;
    QTextCharFormat keyword_format;
    QTextCharFormat comment_line_format;
    QTextCharFormat section_line_format;
    QTextCharFormat format_line_format;
    QTextCharFormat style_line_format;

public slots:
    void addSyntaxKeywords(const QStringList& keywords);
    void addSyntaxKeywords(const QStringList& keywords, const QTextCharFormat& format);
    void addSyntax(const QString& pattern, const QTextCharFormat& format);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    bool isEmpty;
};

enum class HighLighterKeyword{
    r = 255,
    g = 0,
    b = 161,
};

enum class HighLighterSectionLine{
    r = 5,
    g = 40,
    b = 229,
};

enum class HighLighterCommentLine{
    r = 127,
    g = 127,
    b = 127,
};

enum class HighLighterFormatLine{
    r = 173,
    g = 86,
    b = 248,
};

#ifdef QT_DEBUG
enum class HighLighterStyleLine{
    r = 84,
    g = 100,
    b = 0,
};
#endif

#ifdef QT_DEBUG
enum class HighLighterText{
    r = 255,
    g = 255,
    b = 255,
};
#endif

#endif // SYNTAX_HIGHLIGHTER_H
