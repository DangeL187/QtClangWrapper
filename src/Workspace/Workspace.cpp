#include "Workspace/Workspace.hpp"

#include "Application.hpp"

Workspace::Workspace(const QString& title) {
    _title.setText(title);

    initScrollArea();
    initLayout();
    updateLineNumbers();
    QObject::connect(&_text, &QPlainTextEdit::textChanged, this, &Workspace::updateLineNumbers);
}

void Workspace::initLayout() {
    auto* separator = new QWidget();
    separator->setStyleSheet("background-color: rgb(55, 71, 79)");
    separator->setFixedWidth(2);

    int id = QFontDatabase::addApplicationFont("../fonts/JetBrainsMono-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    _font = QFont(family);
    _font.setPointSize(12);
    _font.setStyleHint(QFont::TypeWriter);

    _line_numbers.setStyleSheet("color: rgb(123, 100, 180)");
    _line_numbers.setFont(_font);
    _line_numbers.setAlignment(Qt::AlignRight);

    _text.setPlainText("");
    _text.setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    _text.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _text.document()->setDocumentMargin(0);
    _text.setStyleSheet("border: none");
    _text.setFont(_font);

    _title.setFont(_font);

    _layout = std::make_shared<QVBoxLayout>(&_scroll_area_widget);
    _layout->addWidget(&_title);
    _layout->addLayout(&_h_layout);
    _layout->setContentsMargins(0, 0, 0, 0);

    _line_numbers_layout.setContentsMargins(10, 0, 20, 0);
    _line_numbers_layout.addWidget(&_line_numbers);
    _line_numbers_layout.addStretch(1);

    _h_layout.addLayout(&_line_numbers_layout);
    _h_layout.addWidget(separator);
    _h_layout.addWidget(&_text);
    _h_layout.setContentsMargins(0, 0, 0, 0);
    _h_layout.setSpacing(0);
}

void Workspace::initScrollArea() {
    _scroll_area = std::make_shared<QScrollArea>(this);
    _scroll_area->setFrameShape(QFrame::NoFrame);
    _scroll_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scroll_area->setWidgetResizable(true);
    _scroll_area->setWidget(&_scroll_area_widget);
    _scroll_area->setStyleSheet("background-color: rgb(30, 30, 30)");
}

void Workspace::setText(const QString& text) {
    _text.setPlainText(text);
}

void Workspace::updateLineNumbers() {
    QString text = _text.toPlainText();
    QStringList lines = text.split("\n");
    long long lineCount = lines.count();

    QString lineNumbers;
    for (long long i = 1; i <= lineCount; ++i) {
        lineNumbers += QString::number(i) + "\n";
    }

    _line_numbers.setText(lineNumbers);
}

void Workspace::updateSize(const QSize& size) {
    _scroll_area->setFixedSize(size);
}
