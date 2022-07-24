#ifndef QSSDSYNTAXHIGHLIGHTER_H
#define QSSDSYNTAXHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class QssdSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit QssdSyntaxHighlighter(QTextDocument* parent);

    /*!
     * \brief Implements \a\b QSyntaxHighlighter::highlightBlock() method
     * \param text
     */
    virtual void highlightBlock(const QString& text) override;

private:
    struct HighlightRule
    {
        HighlightRule(const QString& pat, QTextCharFormat form, int cap = 0)
            : pattern(pat), format(form), capture(cap)
        {
        }
        QRegularExpression pattern;
        QTextCharFormat format;
        int capture;
    };

    QVector<HighlightRule> mHlRules;

    QRegularExpression mCommentStartRegex;
    QRegularExpression mCommentEndRegex;
    QTextCharFormat mMultiLnCommentFormat;
};

#endif // QSSDSYNTAXHIGHLIGHTER_H
