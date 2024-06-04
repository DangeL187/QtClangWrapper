#ifndef QTCLANGWRAPPER_PROCESS_HPP
#define QTCLANGWRAPPER_PROCESS_HPP

#include <chrono>
#include <fstream>
#include <memory>

#include <QProcess>

class Application;

class Process: public QProcess {
public:
    explicit Process(Application* parent);
    void genAssembler();
    void genExecutable();
    void genPreprocessed();
    void genIR();

private:
    char            _output_workspace_id = 0;
    Application*    _parent;

    static std::string readFile(const std::string& file_path);
    bool runProcess(const QString& command);

private slots:
    void handleReadyRead();
};

#endif //QTCLANGWRAPPER_PROCESS_HPP
