#include "Application.hpp"

#include "Panel/Panel.hpp"
#include "Process/Process.hpp"
#include "Window.hpp"
#include "Workspace/Workspace.hpp"

Application::Application(int argc, char* argv[]): _application(argc, argv) {
    initPanel();
    initWorkspaces();
    initWindow();
    initLayout();

    _process = std::make_shared<Process>(this);

    // TODO: move to the "Run" button method:
    _process->genPreprocessed();
    _process->genIR();
    _process->genAssembler();
    _process->genExecutable();
}

void Application::decreaseWorkspacesCount() {
    _workspaces_count--;
}

void Application::increaseWorkspacesCount() {
    _workspaces_count++;
}

void Application::initLayout() {
    _main_layout.setContentsMargins(0, 0, 0, 0);
    _main_layout.addWidget(_panel.get());
    _main_layout.addLayout(&_workspaces);
}

void Application::initPanel() {
    _panel = std::make_shared<Panel>(this);
}

void Application::initWindow() {
    auto* centralWidget = new QWidget();
    centralWidget->setLayout(&_main_layout);

    _window = std::make_shared<Window>(this);
    _window->setCentralWidget(centralWidget);

    _window->show();
    //_window->move(960, 240);
    _window->resize(1200, 600);
    _window->setStyleSheet("background-color: rgb(0, 0, 0)");
}

void Application::initWorkspaces() {
    _workspaces.setContentsMargins(10, 10, 10, 10);

    _workspace_asm = std::make_shared<Workspace>("Assembler View");
    _workspace_editor = std::make_shared<Workspace>("Code Editor");
    _workspace_exec = std::make_shared<Workspace>("Executor");
    _workspace_IR = std::make_shared<Workspace>("IR Code View");
    _workspace_prep = std::make_shared<Workspace>("Preprocessed View");

    _workspace_asm->setVisible(false);
    _workspace_IR->setVisible(false);
    _workspace_prep->setVisible(false);

    _workspaces.addWidget(_workspace_editor.get());
    _workspaces.addWidget(_workspace_exec.get());
    _workspaces.addWidget(_workspace_prep.get());
    _workspaces.addWidget(_workspace_IR.get());
    _workspaces.addWidget(_workspace_asm.get());
}

void Application::quit() { // TODO: might be deprecated
    //stop?
    QCoreApplication::quit();
}

void Application::setAsmOutput(const QString& text) {
    _workspace_asm->setText(text);
}
void Application::setAsmVisible(bool value) {
    _workspace_asm->setVisible(value);
}

void Application::setExecOutput(const QString& text) {
    _workspace_exec->setText(text);
}
void Application::setExecVisible(bool value) {
    _workspace_exec->setVisible(value);
}

void Application::setIrOutput(const QString& text) {
    _workspace_IR->setText(text);
}
void Application::setIrVisible(bool value) {
    _workspace_IR->setVisible(value);
}

void Application::setPrepOutput(const QString& text) {
    _workspace_prep->setText(text);
}
void Application::setPrepVisible(bool value) {
    _workspace_prep->setVisible(value);
}

void Application::updateSize(QSize size) {
    if (!size.isValid()) size = _window->size();

    for (int i = 0; i < _workspaces.count(); i++) {
        auto workspace = qobject_cast<Workspace*>(_workspaces.itemAt(i)->widget());
        int h_margins = _workspaces.contentsMargins().left() + _workspaces.contentsMargins().right();
        int v_margins = _workspaces.contentsMargins().top() + _workspaces.contentsMargins().bottom();
        workspace->updateSize({size.width() / _workspaces_count - h_margins, size.height() - v_margins - _panel->height() - 7});
    }
}
