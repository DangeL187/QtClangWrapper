#include "Process/Process.hpp"

#include "Application/Application.hpp"
#include "readFile.hpp"

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
        _parent->appendAsmOutput("<Compilation failed>\n# For more information see the output window");
        _parent->appendDisasmOutput("<Compilation failed>\n# For more information see the output window");
        _parent->appendHeadersOutput("<Compilation failed>\n# For more information see the output window");
        _parent->appendIrOutput("<Compilation failed>\n# For more information see the output window");
        _parent->appendMetricsOutput("<Compilation failed>\n# For more information see the output window");
        return;
    }
    genAssembler();
    genObject();
    genDisassembly();
    genHeaders();
    genExecutable();
    genMetrics();

    _parent->appendAsmOutput(QString::fromStdString(readFile("../data/assembler_code.s")));
    _parent->appendDisasmOutput(QString::fromStdString(readFile("../data/disassembly.txt")));
    _parent->appendHeadersOutput(QString::fromStdString(readFile("../data/headers.txt")));
    _parent->appendIrOutput(QString::fromStdString(readFile("../data/IR_code.ll")));
    _parent->appendMetricsOutput(QString::fromStdString(readFile("../data/metrics.txt")));
    _parent->appendPrepOutput(QString::fromStdString(readFile("../data/preprocessed.cpp")));
}

void Process::genAssembler() {
    runProcess("clang", _additional_toolchain + "-S IR_code.ll -o assembler_code.s");
}

void Process::genDisassembly() {
#ifdef __linux__
    //runProcess("objdump", "-d object_file.o > disassembly.txt"); // this command do not work with QProcess (idk why)
    system("objdump -d ../data/object_file.o > ../data/disassembly.txt");
#endif
}

void Process::genExecutable() { // todo: compile options
    QString extension = ".o";
#ifdef _WIN32
    extension = ".exe";
#endif
    runProcess("clang", _additional_toolchain + "object_file.o -o executable" + extension + " -lstdc++ -static-libgcc");

#ifdef _WIN32
    runProcess("executable" + extension);
#endif
#ifdef __linux__
    runProcess("./executable" + extension);
#endif
}

void Process::genHeaders() {
#ifdef __linux__
    // runProcess("readelf", "-h object_file.o > headers.txt"); // this command do not work with QProcess (idk why)
    system("readelf -h ../data/object_file.o > ../data/headers.txt");
#endif
}

void Process::genIR() {
    runProcess("clang", _additional_toolchain + "-S -emit-llvm preprocessed.cpp -o IR_code.ll");
}

void Process::genMetrics() {
#ifdef __linux__
    // runProcess("objdump", "-t executable.o > metrics.txt"); // this command do not work with QProcess (idk why)
    system("objdump -t ../data/executable.o > ../data/metrics.txt");
#endif
}

void Process::genObject() {
    runProcess("clang", _additional_toolchain + "-c IR_code.ll -o object_file.o");
}

void Process::genPreprocessed() {
    runProcess("clang", _additional_toolchain + "-E source.cpp -o preprocessed.cpp");
}

void Process::handleReadyRead() {
    QString error = readAllStandardError();
    QString output = readAllStandardOutput();

    if (!error.isEmpty()) {
        output = error;
        output.replace("\r", "");
        output.chop(1);
        std::cout << "Error text: " << output.toStdString() << "\n";
        _parent->appendExecOutput(output);
        _is_error = true;
        return;
    }

    _parent->appendExecOutput(output);
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
        std::cerr << "Error!\n";
        return false;
    }
    waitForFinished();
    return true;
}