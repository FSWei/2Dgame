# 2Dgame
 A console 2D horizontal board clearance game implemented in C with class(maybe)



# 编译指南：

## 在windows下
直接编译当前文件夹下的所有.cpp文件，运行生成的.exe文件即可

## 在linux下
因为用到了ncurses库，需要在编译指令的后面加上-lncurses参数，然后运行生成的文件即可

我用的是vscode，vscode中不同操作系统的编译详见.vscode中的launch.json和tasks.json



# 如果您没有使用vscode，请直接在当前文件夹的控制台窗口输入以下指令编译：

## windows：
```bash
g++ -fdiagnostics-color=always -g *.cpp -o exe/game.exe
```
之后game.exe会出现在当前文件夹下的exe文件夹中，双击运行即可

## linux：
```bash
g++ -fdiagnostics-color=always -g *.cpp -o exe/game -lncurses
```
之后game会出现在当前文件夹下的exe文件夹中
然后输入以下指令：
```bash
cd exe
sudo ./game
```
运行即可


# （注意，在linux和windows下运行的时候，要先把控制台调整到足够的大小，再运行，否则会出现显示错误）



# 游戏操作：

WASD上下左右，L冲刺