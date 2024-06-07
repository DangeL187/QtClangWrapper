#ifndef QTCLANGWRAPPER_HIGHLIGHTER_HPP
#define QTCLANGWRAPPER_HIGHLIGHTER_HPP

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter {
Q_OBJECT
public:
    explicit Highlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;
};

#endif //QTCLANGWRAPPER_HIGHLIGHTER_HPP
