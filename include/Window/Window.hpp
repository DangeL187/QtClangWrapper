#ifndef QTCLANGWRAPPER_WINDOW_HPP
#define QTCLANGWRAPPER_WINDOW_HPP

#include <QMainWindow>

class Application;

class Window: public QMainWindow {
public:
    explicit Window(Application* parent);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Application* _parent;
};

#endif //QTCLANGWRAPPER_WINDOW_HPP

