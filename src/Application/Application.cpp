#include "Application/Application.hpp"

#include "Panel/Panel.hpp"
#include "Process/Process.hpp"
#include "Window/Window.hpp"
#include "Workspace/Workspace.hpp"

Application::Application(int argc, char* argv[]): _application(argc, argv) {
    _process = std::make_shared<Process>(this);

    initPanel();
    initWorkspaces();
    initWindow();
    initLayout();
}

void Application::appendAsmOutput(const QString& text) {
    _workspace_asm->appendText(text);
}

void Application::appendDisasmOutput(const QString& text) {
    _workspace_disasm->appendText(text);
}

void Application::appendExecOutput(const QString& text) {
    _workspace_exec->appendText(text);
}

void Application::appendHeadersOutput(const QString& text) {
    _workspace_headers->appendText(text);
}

void Application::appendIrOutput(const QString& text) {
    _workspace_IR->appendText(text);
}

void Application::appendMetricsOutput(const QString& text) {
    _workspace_metrics->appendText(text);
}

void Application::appendPrepOutput(const QString& text) {
    _workspace_prep->appendText(text);
}

void Application::clearOutputs() { // TODO: replace with a loop through _workspaces
    _workspace_asm->clearText();
    _workspace_disasm->clearText();
    _workspace_exec->clearText();
    _workspace_headers->clearText();
    _workspace_IR->clearText();
    _workspace_metrics->clearText();
    _workspace_prep->clearText();
}

void Application::compile() {
    std::thread thread(&Application::compileProcess, this);
    thread.detach();
}

// IMPORTANT
// Here happens some error: QObject: Cannot create children for a parent that is in a different thread.
// Why: thread tries to change some variables that are related to another thread
// Seems like it has no effect
void Application::compileProcess() {
    _workspace_editor->saveFile();
    clearOutputs();
    _process->compile();
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
    _workspace_disasm = std::make_shared<Workspace>("Disassembly View");
    _workspace_editor = std::make_shared<Workspace>("Code Editor");
    _workspace_exec = std::make_shared<Workspace>("Executor");
    _workspace_headers = std::make_shared<Workspace>("Headers View");
    _workspace_IR = std::make_shared<Workspace>("IR Code View");
    _workspace_metrics = std::make_shared<Workspace>("Metrics View");
    _workspace_prep = std::make_shared<Workspace>("Preprocessed View");

    _workspace_asm->setVisible(false);
    _workspace_disasm->setVisible(false);
    _workspace_headers->setVisible(false);
    _workspace_IR->setVisible(false);
    _workspace_metrics->setVisible(false);
    _workspace_prep->setVisible(false);

    _workspaces.addWidget(_workspace_editor.get());
    _workspaces.addWidget(_workspace_exec.get());
    _workspaces.addWidget(_workspace_prep.get());
    _workspaces.addWidget(_workspace_IR.get());
    _workspaces.addWidget(_workspace_asm.get());
    _workspaces.addWidget(_workspace_disasm.get());
    _workspaces.addWidget(_workspace_headers.get());
    _workspaces.addWidget(_workspace_metrics.get());
}

void Application::saveOutputFiles(const QString& folder_path) {
    _workspace_asm->saveFile(folder_path + "/assembler_code.s");
    _workspace_disasm->saveFile(folder_path + "/disassembly.txt");
    _workspace_exec->saveFile(folder_path + "/output.txt");
    _workspace_headers->saveFile(folder_path + "/headers.txt");
    _workspace_IR->saveFile(folder_path + "/IR_code.ll");
    _workspace_metrics->saveFile(folder_path + "/metrics.txt");
    _workspace_prep->saveFile(folder_path + "/preprocessed.cpp");
}

void Application::setAsmVisible(bool value) {
    _workspace_asm->setVisible(value);
}

void Application::setCompilerOptions(const QString& options) {
    _process->setCompilerOptions(options);
}

void Application::setLinkerOptions(const QString& options) {
    _process->setLinkerOptions(options);
}

void Application::setDisasmVisible(bool value) {
    _workspace_disasm->setVisible(value);
}

void Application::setEditVisible(bool value) {
    _workspace_editor->setVisible(value);
}

void Application::setExecVisible(bool value) {
    _workspace_exec->setVisible(value);
}

void Application::setHeadersVisible(bool value) {
    _workspace_headers->setVisible(value);
}

void Application::setIrVisible(bool value) {
    _workspace_IR->setVisible(value);
}

void Application::setMetricsVisible(bool value) {
    _workspace_metrics->setVisible(value);
}

void Application::setPrepVisible(bool value) {
    _workspace_prep->setVisible(value);
}

void Application::setSourceFile(const QString& file_path) {
    _workspace_editor->setFilePath(file_path);
}

void Application::updateSize(QSize size) {
    if (_workspaces_count == 0) return;

    if (!size.isValid()) size = _window->size();

    for (int i = 0; i < _workspaces.count(); i++) {
        auto workspace = qobject_cast<Workspace*>(_workspaces.itemAt(i)->widget());
        int h_margins = _workspaces.contentsMargins().left() + _workspaces.contentsMargins().right();
        int v_margins = _workspaces.contentsMargins().top() + _workspaces.contentsMargins().bottom();
        workspace->updateSize({size.width() / _workspaces_count - h_margins, size.height() - v_margins - _panel->height() - 7});
    }
}
