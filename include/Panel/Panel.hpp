#ifndef QTCLANGWRAPPER_PANEL_HPP
#define QTCLANGWRAPPER_PANEL_HPP

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QStandardItemModel>

class Application;

class Panel: public QFrame {
Q_OBJECT
public:
    explicit Panel(Application* parent);

private:
    std::shared_ptr<QHBoxLayout>    _layout;
    Application*                    _parent;
    std::shared_ptr<QComboBox>      _view_selector;

    void initLayout();
    void initOptions();
};

#endif //QTCLANGWRAPPER_PANEL_HPP
