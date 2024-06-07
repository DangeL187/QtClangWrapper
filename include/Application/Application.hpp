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

    void appendAsmOutput(const QString& text);
    void appendDisasmOutput(const QString& text);
    void appendExecOutput(const QString& text);
    void appendHeadersOutput(const QString& text);
    void appendIrOutput(const QString& text);
    void appendMetricsOutput(const QString& text);
    void appendPrepOutput(const QString& text);
    void clearOutputs();
    void compile();
    void decreaseWorkspacesCount();
    void increaseWorkspacesCount();
    void saveOutputFiles(const QString& folder_path);
    void setAsmVisible(bool value);
    void setDisasmVisible(bool value);
    void setEditVisible(bool value);
    void setExecVisible(bool value);
    void setHeadersVisible(bool value);
    void setIrVisible(bool value);
    void setMetricsVisible(bool value);
    void setPrepVisible(bool value);
    void setSourceFile(const QString& file_path);

    void setCompilerOptions(const QString& options);
    void setLinkerOptions(const QString& options);

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
    std::shared_ptr<Workspace>  _workspace_disasm;
    std::shared_ptr<Workspace>  _workspace_editor;
    std::shared_ptr<Workspace>  _workspace_exec;
    std::shared_ptr<Workspace>  _workspace_headers;
    std::shared_ptr<Workspace>  _workspace_IR;
    std::shared_ptr<Workspace>  _workspace_metrics;
    std::shared_ptr<Workspace>  _workspace_prep;
    /*===============================================================*/

    /*======================== initializers =========================*/
    void initLayout();
    void initPanel();
    void initWindow();
    void initWorkspaces();
    /*===============================================================*/

    void compileProcess();
};

#endif //QTCLANGWRAPPER_APPLICATION_HPP
