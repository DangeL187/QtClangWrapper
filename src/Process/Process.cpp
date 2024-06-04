#include "Process/Process.hpp"

#include "Application.hpp"

#include <iostream>

Process::Process(Application* parent): _parent(parent) {
    setEnvironment(QProcess::systemEnvironment());
    setProcessChannelMode(QProcess::SeparateChannels);
    setWorkingDirectory("../data");

    connect(this, &QProcess::readyReadStandardOutput, this, &Process::handleReadyRead);
    connect(this, &QProcess::readyReadStandardError, this, &Process::handleReadyRead);
}

void Process::genAssembler() {
    _output_workspace_id = 4;
    runProcess("clang --target=x86_64-w64-mingw64 -S IR_code.ll -o assembler_code.s");
    _parent->setAsmOutput(QString::fromStdString(readFile("../data/assembler_code.s")));
}

void Process::genExecutable() { // todo: flags
    _output_workspace_id = 1;
    runProcess("clang --target=x86_64-w64-mingw64 -c IR_code.ll -o object_file.o");
    runProcess("clang --target=x86_64-w64-mingw64 object_file.o -o executable.exe -lstdc++ -static-libgcc");
    runProcess("executable.exe");
}

void Process::genIR() {
    _output_workspace_id = 3;
    runProcess("clang --target=x86_64-w64-mingw64 -S -emit-llvm preprocessed.cpp -o IR_code.ll");
    _parent->setIrOutput(QString::fromStdString(readFile("../data/IR_code.ll")));
}

void Process::genPreprocessed() {
    _output_workspace_id = 2;
    runProcess("clang --target=x86_64-w64-mingw64 -E source.cpp -o preprocessed.cpp");
    _parent->setPrepOutput(QString::fromStdString(readFile("../data/preprocessed.cpp")));
}

void Process::handleReadyRead() {
    //std::string output = QString::fromLocal8Bit(_process->readAllStandardOutput()).toStdString(); // todo: might be deprecated
    QString error = readAllStandardError();
    QString output = readAllStandardOutput();

    if (output.isEmpty()) output = error;
    output.replace("\r", "");

    if (_output_workspace_id == 1) _parent->setExecOutput(output);
    else if (_output_workspace_id == 2) _parent->setPrepOutput(output);
    else if (_output_workspace_id == 3) _parent->setIrOutput(output);
    else if (_output_workspace_id == 4) _parent->setAsmOutput(output);
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

bool Process::runProcess(const QString& command) {
    if (command.isEmpty()) return false;

    closeWriteChannel();
    close();

#ifdef _WIN32
    start("cmd", {"/s", "/v", "/c", command});
#endif
#ifdef __linux__
    long long space_index = command.indexOf(' ');
    if (space_index != -1) {
        _process->start(command.left(space_index), {command.mid(space_index + 1)});
    } else {
        _process->start(command);
    }
#endif

    if (!waitForStarted()) {
        std::cout << "Error\n";
        return false;
    }
    waitForFinished();
    return true;
}