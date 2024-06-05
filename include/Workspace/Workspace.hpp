#ifndef QTCLANGWRAPPER_WORKSPACE_HPP
#define QTCLANGWRAPPER_WORKSPACE_HPP

#include <QFile>
#include <QFrame>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include <QScrollArea>
#include <QVBoxLayout>

class Workspace: public QFrame {
Q_OBJECT
public:
    explicit Workspace(const QString& title);

    void appendText(const QString& text);
    void clearText();
    void loadFile();
    void saveFile(QString file_path = "");
    void setFilePath(const QString& file_path = "");
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
    QString                         _file_path;
    QPlainTextEdit                  _text;
    QLabel                          _title;
    /*==========================================================*/

    void initFont();
    void initText();
    void initLineNumbers();
    void initLayout();
    void initScrollArea();

private slots:
    void updateLineNumbers();
};

#endif //QTCLANGWRAPPER_WORKSPACE_HPP
