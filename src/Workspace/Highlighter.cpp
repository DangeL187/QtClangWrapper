#include "Workspace/Highlighter.hpp"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {

    HighlightingRule rule;

    keywordFormat.setForeground(Qt::yellow);
    const QString keywordPatterns[] = {
            QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
            QStringLiteral("\\breturn\\b"), QStringLiteral("\\bvoid\\b"),
            QStringLiteral("\\bint\\b"), QStringLiteral("\\bfloat\\b"),
            QStringLiteral("\\bdouble\\b"), QStringLiteral("\\bbool\\b"),
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bif\\b"),
            QStringLiteral("\\belse\\b"), QStringLiteral("\\bfor\\b"),
            QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bdo\\b"),
            QStringLiteral("\\bswitch\\b"), QStringLiteral("\\bcase\\b"),
            QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bnamespace\\b"),
            QStringLiteral("\\busing\\b"), QStringLiteral("\\btry\\b"),
            QStringLiteral("\\bcatch\\b"), QStringLiteral("\\bthrow\\b"),
            QStringLiteral("\\bnew\\b"), QStringLiteral("\\bdelete\\b"),
            QStringLiteral("\\bthis\\b"), QStringLiteral("\\bsizeof\\b"),
            QStringLiteral("\\btypedef\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\benum\\b"), QStringLiteral("\\bunion\\b"),
            QStringLiteral("\\bvirtual\\b"), QStringLiteral("\\bpublic\\b"),
            QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bprivate\\b"),
            QStringLiteral("\\bfriend\\b"), QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\binline\\b"), QStringLiteral("\\bextern\\b"),
            QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bconstexpr\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setForeground(Qt::magenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setForeground(Qt::magenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    preprocessorFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression(QStringLiteral("#\\b(include|define|ifdef|ifndef|endif|else|elif)\\b"));
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString& text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
