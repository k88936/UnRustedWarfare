set QT_DIR=C:\Qt\6.8.2\msvc2022_64
cmake -DCMAKE_BUILD_TYPE=Release  -DCMAKE_TOOLCHAIN_FILE=%QT_DIR%\lib\cmake\Qt6\qt.toolchain.cmake  -DCMAKE_CXX_FLAGS="-std:c++20 -Zc:__cplusplus" -S . -B build

cmake --build build --target stainless_warfare
cp build\Debug\stainless_warfare.exe build

%QT_DIR%\bin\windeployqt.exe build\\stainless_warfare.exe
mkdir build\plugins
robocopy build\multimedia build\plugins /E