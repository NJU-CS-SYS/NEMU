# NEMU

计算机系统基础 PA & 系统方向综合实验组成部分

## 目标

精简实现以适应自制的 MIPS 流水线 CPU，解释执行内核和自制编译器生成的可执行程序。

## 构建说明

1. `make nemu_on_npc` 以 mips 架构为目标平台编译 NEMU 并链接对应的标准库
2. `make nemu_on_npc.o` 同上，但是只编译 NEMU 整体为可重定位文件，不链接标准库
3. `make nemu` 以当前系统为目标平台编译可执行的 NEMU
4. `make monitor` 编译与 NEMU 交互的外设模拟器

`include/common.h` 中如果定义 `SYS_LAB` 宏，最主要的变化，就是标准输入输出被“重载”成自行实现的接口，
以和 monitor 模块进行交互。

目标 `nemu_on_npc` 和 `nemu_on_npc.o` 在编译选项中定义了 `DEPLOY` 宏，用于进一步脱离系统环境。
由于宏定义在选项里，且切换架构时不同架构的 `.o` 文件无法链接到一起，所以先 `make clean` 一下是必要的。

## 模拟外设

NPC 完全采用内存映射方式进行 IO 操作，目前使用 monitor 模块模拟内存映射 IO 和中断事件以测试 NEMU。

### 使用方法

```
$ make nemu
$ make monitor
# 下面 4 条命令在独立的终端中依序分别执行
$ ./monitor.bin monitor/config key   # 启动键盘
$ ./monitor.bin monitor/config scr   # 启动屏幕
$ ./monitor.bin monitor/config timer # 启动定时器
$ ./nemu                             # 带参数启动 nemu
```

`config` 文件记录了相关的设定，如屏幕尺寸等，格式如下：

```
屏幕横向分辨率
屏幕纵向分辨率
字体横向宽度
字体纵向宽度
屏幕内存映射使用的文件名
键盘内存映射使用的文件名
NEMU PID 的文件名
```

主要需要修改的就是根据自己的操作环境，调整横向分辨率和纵向分辨率，
因为 monitor, 以及我们的 vga 文字模式硬件控制器，都是使用填充空格来达到换行效果的。
