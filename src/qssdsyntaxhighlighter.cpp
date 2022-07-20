#include "qssdsyntaxhighlighter.h"

QssdSyntaxHighlighter::QssdSyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    // Class highlight rule
    QTextCharFormat classFormat;
    classFormat.setForeground(QColor(Qt::darkRed));
    classFormat.setFontWeight(QFont::Bold);
    mHlRules.emplaceBack(R"((^|[\s\};])([A-Z]\w*)\b)", classFormat, 2);

    // Variables highlight rule
    QTextCharFormat varFormat;
    varFormat.setForeground(Qt::darkMagenta);
    mHlRules.emplaceBack(R"((^|\B)(\$[a-zA-Z]\w*))", varFormat, 2);

    // Color hash format
    QTextCharFormat colorHashFormat;
    colorHashFormat.setForeground(QColor(Qt::darkBlue));
    colorHashFormat.setFontItalic(true);
    mHlRules.emplaceBack(R"(\B#([a-fA-F0-9]{2}){3,4}\b)", colorHashFormat);

    // Length format
    QTextCharFormat lengthFormat;
    lengthFormat.setForeground(QColor(Qt::darkYellow));
    mHlRules.emplaceBack(R"(\b\d+(px|pt|em|ex)\b)", lengthFormat);

    // Strings literals
    QTextCharFormat stringLiteral;
    stringLiteral.setForeground(QColor("#c27800"));
    stringLiteral.setFontWeight(QFont::Bold);
    mHlRules.emplaceBack(R"(\".*\")", stringLiteral);
}

void QssdSyntaxHighlighter::highlightBlock(const QString& text)
{
    for (const auto& rule : qAsConst(mHlRules)) {
        auto matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            auto match = matchIterator.next();
            setFormat(match.capturedStart(rule.capture),
                match.capturedLength(rule.capture), rule.format);
        }
    }
}
