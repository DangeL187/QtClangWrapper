#ifndef QTCLANGWRAPPER_APPLICATION_HPP
#define QTCLANGWRAPPER_APPLICATION_HPP

#include <memory>
#include <thread>

#include <QApplication>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QWidget>

class Panel;
class Process;
class Window;
class Workspace;

class Application {
public:
    Application(int argc, char* argv[]);

    void quit();

    void setAsmOutput(const QString& text);
    void setAsmVisible(bool value);
    void setExecOutput(const QString& text);
    void setExecVisible(bool value);
    void setIrOutput(const QString& text);
    void setIrVisible(bool value);
    void setPrepOutput(const QString& text);
    void setPrepVisible(bool value);
    void decreaseWorkspacesCount();
    void increaseWorkspacesCount();

    void updateSize(QSize size = {});

private:
    /*========================= Application =========================*/
    QApplication                _application;
    QVBoxLayout                 _main_layout;
    std::shared_ptr<Panel>      _panel;
    std::shared_ptr<Process>    _process;
    std::shared_ptr<Window>     _window;
    /*===============================================================*/

    /*========================= Workspaces ==========================*/
    QHBoxLayout                 _workspaces;
    short                       _workspaces_count = 2;
    std::shared_ptr<Workspace>  _workspace_asm;
    std::shared_ptr<Workspace>  _workspace_editor;
    std::shared_ptr<Workspace>  _workspace_exec;
    std::shared_ptr<Workspace>  _workspace_IR;
    std::shared_ptr<Workspace>  _workspace_prep;
    /*===============================================================*/

    /*======================== initializers =========================*/
    void initLayout();
    void initPanel();
    void initWindow();
    void initWorkspaces();
    /*===============================================================*/
};

#endif //QTCLANGWRAPPER_APPLICATION_HPP
