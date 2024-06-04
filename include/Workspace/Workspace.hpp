#ifndef QTCLANGWRAPPER_WORKSPACE_HPP
#define QTCLANGWRAPPER_WORKSPACE_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include <QScrollArea>

class Workspace: public QFrame {
Q_OBJECT
public:
    explicit Workspace(const QString& title);

    void setText(const QString& text);
    void updateSize(const QSize& size);

private:
    /*===================== Layout Related =====================*/
    std::shared_ptr<QVBoxLayout>    _layout;
    QHBoxLayout                     _h_layout;
    QVBoxLayout                     _line_numbers_layout;
    std::shared_ptr<QScrollArea>    _scroll_area;
    QWidget                         _scroll_area_widget;
    /*==========================================================*/

    /*==================== Workspace Related ===================*/
    QFont                           _font;
    QLabel                          _line_numbers;
    QPlainTextEdit                  _text;
    QLabel                          _title;
    /*==========================================================*/

    void initLayout();
    void initScrollArea();

private slots:
    void updateLineNumbers();
};

#endif //QTCLANGWRAPPER_WORKSPACE_HPP
