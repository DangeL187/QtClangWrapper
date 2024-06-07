#ifndef QTCLANGWRAPPER_PROCESS_HPP
#define QTCLANGWRAPPER_PROCESS_HPP

#include <chrono>
#include <memory>

#include <QProcess>

class Application;

class Process: public QProcess {
public:
    explicit Process(Application* parent);
    void setCompilerOptions(QString options);
    void setLinkerOptions(QString options);
    void compile();

private:
    QString         _args_compiler;
    QString         _args_linker;
    bool            _is_error = false;
    Application*    _parent;

    void genAssembler();
    void genDisassembly();
    void genExecutable();
    void genHeaders();
    void genIR();
    void genObject();
    void genPreprocessed();
    bool runProcess(const QString& command, const QString& args = "", const QString& output_file = "");

private slots:
    void handleReadyRead();
};

#endif //QTCLANGWRAPPER_PROCESS_HPP
