#ifndef QTCLANGWRAPPER_PANEL_HPP
#define QTCLANGWRAPPER_PANEL_HPP

#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>

class Application;
class CustomComboBox;

class Panel: public QFrame {
Q_OBJECT
public:
    explicit Panel(Application* parent);

private:
    std::shared_ptr<QHBoxLayout>    _layout;
    std::shared_ptr<QPushButton>    _open_file_button;
    Application*                    _parent;
    std::shared_ptr<CustomComboBox> _options;
    std::shared_ptr<QPushButton>    _run_button;
    std::shared_ptr<QPushButton>    _save_button;
    std::shared_ptr<QComboBox>      _view_selector;

    void initLayout();
    void initOpenFileButton();
    void initOptions();
    void initRunButton();
    void initSaveButton();
    void initSelector();
};

#endif //QTCLANGWRAPPER_PANEL_HPP
