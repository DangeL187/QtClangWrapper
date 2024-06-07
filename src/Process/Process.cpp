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
    genExecutable();
    genHeaders();

    _parent->appendAsmOutput(QString::fromStdString(readFile("../data/assembler_code.s")));
    _parent->appendDisasmOutput(QString::fromStdString(readFile("../data/disassembly.txt")));
    _parent->appendHeadersOutput(QString::fromStdString(readFile("../data/headers.txt")));
    _parent->appendIrOutput(QString::fromStdString(readFile("../data/IR_code.ll")));
    _parent->appendPrepOutput(QString::fromStdString(readFile("../data/preprocessed.cpp")));
}

void Process::genAssembler() {
    auto start_time = std::chrono::high_resolution_clock::now();
    runProcess("clang", _args_compiler + "-S IR_code.ll -o assembler_code.s");
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Assembler generation finished in " + QString::number(time));
}

void Process::genDisassembly() {
    auto start_time = std::chrono::high_resolution_clock::now();
    runProcess("objdump", "-d object_file.o", workingDirectory() + "/disassembly.txt");
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Disassembly generation finished in " + QString::number(time));
}

void Process::genExecutable() {
    auto start_time = std::chrono::high_resolution_clock::now();
    QString extension = ".o";
#ifdef _WIN32
    extension = ".exe";
#endif
    runProcess("clang", _args_compiler + "object_file.o -o executable" + extension + " " + _args_linker);

#ifdef _WIN32
    runProcess("executable" + extension);
#endif
#ifdef __linux__
    runProcess("./executable" + extension);
#endif
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Executable generation finished in " + QString::number(time));
}

void Process::genHeaders() {
    auto start_time = std::chrono::high_resolution_clock::now();
#ifdef _WIN32
    runProcess("objdump", "-x executable.exe", workingDirectory() + "/headers.txt");
#endif
#ifdef __linux__
    runProcess("readelf", "-h object_file.o", workingDirectory() + "/headers.txt");
#endif
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Headers generation finished in " + QString::number(time));
}

void Process::genIR() {
    auto start_time = std::chrono::high_resolution_clock::now();
    runProcess("clang", _args_compiler + "-S -emit-llvm preprocessed.cpp -o IR_code.ll");
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("IR code generation finished in " + QString::number(time));
}

void Process::genObject() {
    auto start_time = std::chrono::high_resolution_clock::now();
    runProcess("clang", _args_compiler + "-c IR_code.ll -o object_file.o");
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Object file generation finished in " + QString::number(time));
}

void Process::genPreprocessed() {
    auto start_time = std::chrono::high_resolution_clock::now();
    runProcess("clang", _args_compiler + "-E source.cpp -o preprocessed.cpp");
    double time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count();
    _parent->appendMetricsOutput("Preprocessing finished in " + QString::number(time));
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

bool Process::runProcess(const QString& command, const QString& args, const QString& output_file) {
    if (command.isEmpty()) return false;

    closeWriteChannel();
    close();

    if (!output_file.isEmpty()) setStandardOutputFile(output_file);

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
    setStandardOutputFile(QString());
    return true;
}

void Process::setCompilerOptions(QString options) {
    options = options.trimmed();
    options.append(' ');
    _args_compiler = options;
}

void Process::setLinkerOptions(QString options) {
    options = options.trimmed();
    options.append(' ');
    _args_linker = options;
}
