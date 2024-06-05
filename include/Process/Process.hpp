#ifndef QTCLANGWRAPPER_PROCESS_HPP
#define QTCLANGWRAPPER_PROCESS_HPP

#include <chrono>
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
    void genDisassembly();
    void genExecutable();
    void genHeaders();
    void genIR();
    void genMetrics();
    void genObject();
    void genPreprocessed();
    bool runProcess(const QString& command, const QString& args = "");

private slots:
    void handleReadyRead();
};

#endif //QTCLANGWRAPPER_PROCESS_HPP
