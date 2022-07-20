#include "qssdsyntaxhighlighter.h"

QssdSyntaxHighlighter::QssdSyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    // Multiline comment formtat and regexes
    mMultiLnCommentFormat.setForeground(Qt::darkGreen);
    mMultiLnCommentFormat.setFontItalic(true);
    mCommentStartRegex = QRegularExpression(R"(/\*)");
    mCommentEndRegex = QRegularExpression(R"(\*/)");

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
    mHlRules.emplaceBack(R"(".[^"\n]*")", stringLiteral);

    // Fnction call format
    QTextCharFormat funcCallFormat;
    funcCallFormat.setForeground(Qt::darkBlue);
    mHlRules.emplaceBack(R"(([a-zA-z_]\w*)\((.*)(\)))", funcCallFormat, 1);
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

    // Finding multiline comment
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(mCommentStartRegex);

    while (startIndex >= 0) {
        QRegularExpressionMatch match
            = mCommentEndRegex.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, mMultiLnCommentFormat);
        startIndex
            = text.indexOf(mCommentStartRegex, startIndex + commentLength);
    }
}
