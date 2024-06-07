# QtClangWrapper
Crossplatform UI Wrapper for Clang Compiler

<img src="https://github.com/DangeL187/QtClangWrapper/blob/main/demo.png">

# Features
* Shows preprocessed code
* Shows IR code
* Shows assembler code
* Shows program output
* Shows compiler warnings and errors
* Shows the time spent on each stage
* Shows disassembled code with `objdump`
* Shows output of the `readelf -h` (`objdump -x` for Windows)
* Supports compiler and linker args
* Syntax highlighter
* The ability to save results in a separate folder
* The ability to work with any `.cpp` file

# Building
Regarding setting up Qt, please refer to the [CMakeLists.txt](https://github.com/DangeL187/QtClangWrapper/blob/main/CMakeLists.txt) file.

```
mkdir build
cd build
cmake ..
make
```

