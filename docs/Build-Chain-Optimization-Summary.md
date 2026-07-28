# 新链路优化总结

新链路统一使用 CMake + Ninja 完成 Windows x64 Debug 配置、编译和部署，支持 Qt 5.14.1、VTK 8.2 依赖自动发现。首次通过 setup.ps1 保存本机配置，后续 build.ps1 和 run.ps1 均为零参数执行。

构建脚本会自动加载 VS2017 x64 开发环境，并在配置前验证 cl.exe、INCLUDE、LIB、C++ 标准库头文件和 Ninja，不再依赖历史增量产物或预先打开的 Developer PowerShell。

主要优化：移除 E 盘、junction、手工 PATH 和每次指定运行目录；自动部署 Qt/VTK/项目 DLL 及 Debug 平台插件 qwindowsd.dll；增加环境检查、运行库检查和部署完整性校验；修复 VS2017/CMake 3.18 兼容性、资源文件和 Windows 宏冲突问题；保留旧链路脚本，便于回归验证。

使用方式：在 PowerShell 中进入 D:\Project，首次执行 .\scripts\setup.ps1，之后执行 .\scripts\build.ps1 编译部署，执行 .\scripts\run.ps1 启动程序。若执行策略未配置，当前用户执行一次 Set-ExecutionPolicy -Scope CurrentUser RemoteSigned -Force。
