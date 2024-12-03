.PHONY:mingw linux msvc
projectName=$(shell basename $(CURDIR))
projectPath=D:\\linux$(subst /,\\,$(shell pwd))
QtVersion=6.7.3
linux:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_BUILD_PROGRAME=make -DCMAKE_TOOLCHAIN_FILE=/opt/Qt/$(QtVersion)/gcc_64/lib/cmake/Qt6/qt.toolchain.cmake -G "Unix Makefiles" -S . -B ./cmake-build-release
	cmake --build cmake-build-release --target all
testOnLinux: linux
	cd cmake-build-release/ && ./$(projectName)
msvc:
	mkdir -p /mnt/d/linux$(shell pwd)
	powershell.exe -c  "& 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1' -SkipAutomaticLocation;cmake -DCMAKE_BUILD_TYPE=Release  -DCMAKE_TOOLCHAIN_FILE=C:\Qt\$(QtVersion)\msvc2019_64\lib\cmake\Qt6\qt.toolchain.cmake  -DCMAKE_CXX_FLAGS='-std:c++20 -Zc:__cplusplus' -S . -B $(projectPath)\\cmake-build-msvc"
	powershell.exe -c  "& 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1' -SkipAutomaticLocation;msbuild $(projectPath)\\cmake-build-msvc\\$(projectName).sln /p:Configuration=Release"
	/mnt/c/Qt/$(QtVersion)/msvc2019_64/bin/windeployqt.exe $(projectPath)\\cmake-build-msvc\\Release\\$(projectName).exe
