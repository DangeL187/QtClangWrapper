#include "Process/Process.hpp"

#include "Application.hpp"

#include <iostream>

Process::Process(Application* parent): _parent(parent) {
    setEnvironment(QProcess::systemEnvironment());
    setProcessChannelMode(QProcess::SeparateChannels);
    setWorkingDirectory("../data");

    connect(this, &QProcess::readyReadStandardOutput, this, &Process::handleReadyRead);
    connect(this, &QProcess::readyReadStandardError, this, &Process::handleReadyRead);

#ifdef _WIN32
    _additional_toolchain = "--target=x86_64-w64-mingw64 "; // TODO: make customizable
#endif
}

void Process::compile() {
    _is_error = false;

    genPreprocessed();
    genIR();
    if (_is_error) {
        _parent->appendIrOutput("<Compilation failed>\n# For more information see the output window");
        _parent->appendAsmOutput("<Compilation failed>\n# For more information see the output window");
        return;
    }
    genAssembler();
    genExecutable();
}

void Process::genAssembler() {
    runProcess("clang", _additional_toolchain + "-S IR_code.ll -o assembler_code.s");
}

void Process::genExecutable() { // todo: compile options
    QString extension = ".o";
#ifdef _WIN32
    extension = ".exe";
#endif
    runProcess("clang", _additional_toolchain + "-c IR_code.ll -o object_file.o");
    runProcess("clang", _additional_toolchain + "object_file.o -o executable" + extension + " -lstdc++ -static-libgcc");

#ifdef _WIN32
    runProcess("executable" + extension);
#endif
#ifdef __linux__
    runProcess("./executable" + extension);
#endif
}

void Process::genIR() {
    runProcess("clang", _additional_toolchain + "-S -emit-llvm preprocessed.cpp -o IR_code.ll");
}

void Process::genPreprocessed() {
    runProcess("clang", _additional_toolchain + "-E source.cpp -o preprocessed.cpp");
}

void Process::handleReadyRead() {
    //std::string output = QString::fromLocal8Bit(_process->readAllStandardOutput()).toStdString(); // todo: might be deprecated
    QString error = readAllStandardError();
    QString output = readAllStandardOutput();

    if (output.isEmpty()) {
        output = error;
        output.replace("\r", "");
        output.chop(1);
        _parent->appendExecOutput(output);
        _is_error = true;
        return;
    }

    _parent->appendExecOutput(output);
    _parent->appendPrepOutput(QString::fromStdString(readFile("../data/preprocessed.cpp")));
    _parent->appendIrOutput(QString::fromStdString(readFile("../data/IR_code.ll")));
    _parent->appendAsmOutput(QString::fromStdString(readFile("../data/assembler_code.s")));
}

std::string Process::readFile(const std::string& file_path) {
    std::string output;

    std::ifstream file(file_path);
    if (!file.is_open()) return "File " + file_path + " not found";

    std::string line;
    while (std::getline(file, line)) {
        output += line + "\n";
    }
    file.close();

    return output;
}

bool Process::runProcess(const QString& command, const QString& args) {
    if (command.isEmpty()) return false;

    closeWriteChannel();
    close();

#ifdef _WIN32
    start("cmd", {"/s", "/v", "/c", command + " " + args});
#endif
#ifdef __linux__
    start(command, args.split(" "));
#endif

    if (!waitForStarted()) {
        std::cerr << "Error:\n";
        return false;
    }
    waitForFinished();
    return true;
}