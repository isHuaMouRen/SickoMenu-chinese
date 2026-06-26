@echo off

:: 替换为你自己的msbuild路径
"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" SickoMenu.vcxproj -property:Configuration=Release -property:Platform=x86 -p:AdditionalOptions="/utf-8 %%%%(AdditionalOptions)" /m
"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" SickoMenu.vcxproj -property:Configuration=Release_Version -property:Platform=x86 -p:AdditionalOptions="/utf-8 %%%%(AdditionalOptions)" /m
"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" SickoMenu.vcxproj -property:Configuration=Debug -property:Platform=x86 -p:AdditionalOptions="/utf-8 %%%%(AdditionalOptions)" /m
"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" SickoMenu.vcxproj -property:Configuration=Debug_Version -property:Platform=x86 -p:AdditionalOptions="/utf-8 %%%%(AdditionalOptions)" /m

:: 复制成品至游戏目录 方便调试（你可以删除这段代码）
copy "Debug\SickoMenu.dll" "Z:\Steam\steamapps\common\Among Us"
copy "Debug_Version\version.dll" "Z:\Steam\steamapps\common\Among Us"
