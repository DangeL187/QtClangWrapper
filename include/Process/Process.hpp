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
    void compile();

private:
    QString         _additional_toolchain;
    bool            _is_error = false;
    Application*    _parent;

    void genAssembler();
    void genExecutable();
    void genPreprocessed();
    void genIR();
    static std::string readFile(const std::string& file_path);
    bool runProcess(const QString& command, const QString& args = "");

private slots:
    void handleReadyRead();
};

#endif //QTCLANGWRAPPER_PROCESS_HPP
