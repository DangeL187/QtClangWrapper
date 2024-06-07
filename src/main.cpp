#include "Application/Application.hpp"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    system("chcp 65001"); //windows required
#endif

    Application app(argc, argv);
    return QApplication::exec();
}
