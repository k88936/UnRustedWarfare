.PHONY: sync build deploy-release deploy-debug
projectName=$(shell basename $(CURDIR))
projectPath=D:\\linux$(subst /,\\,$(shell pwd))
QtVersion=6.8.1
sync:
	mkdir -p /mnt/d/linux$(shell pwd)
	rsync -rv --progress --exclude={".git/","cmake-build-*",".idea" ,".venv/"} ./ /mnt/d/linux$(shell pwd)
build:
	powershell.exe -c  "& 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1' -SkipAutomaticLocation;cmake -DCMAKE_BUILD_TYPE=Release  -DCMAKE_TOOLCHAIN_FILE=C:\Qt\$(QtVersion)\msvc2022_64\lib\cmake\Qt6\qt.toolchain.cmake  -DCMAKE_CXX_FLAGS='-std:c++20 -Zc:__cplusplus' -S $(projectPath) -B $(projectPath)\\cmake-build-msvc"
	powershell.exe -c  "& 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1' -SkipAutomaticLocation;msbuild $(projectPath)\\cmake-build-msvc\\ALL_BUILD.vcxproj /p:Configuration=Release"
deploy-release:
	/mnt/c/Qt/$(QtVersion)/msvc2022_64/bin/windeployqt.exe $(projectPath)\\cmake-build-msvc\\Release\\
	mkdir -p /mnt/d/linux$(shell pwd)/cmake-build-msvc/Release/plugins
	mv /mnt/d/linux$(shell pwd)/cmake-build-msvc/Release/multimedia /mnt/d/linux$(shell pwd)/cmake-build-msvc/Release/plugins
deploy-debug:
	/mnt/c/Qt/$(QtVersion)/msvc2022_64/bin/windeployqt.exe $(projectPath)\\cmake-build-msvc\\Debug\\
	mkdir -p /mnt/d/linux$(shell pwd)/cmake-build-msvc/Debug/plugins
	mv /mnt/d/linux$(shell pwd)/cmake-build-msvc/Debug/multimedia /mnt/d/linux$(shell pwd)/cmake-build-msvc/Release/plugins
