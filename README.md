# NEMU

计算机系统基础 PA & 系统方向综合实验组成部分

## 目标

精简实现以适应自制的 MIPS 流水线 CPU，解释执行内核和自制编译器生成的可执行程序。

## TODO

- [ ] 删除 cache，分段和分页机制
- [ ] 修改内核，适应简化的模拟器

## 模拟外设

NPC 完全采用内存映射方式进行 IO 操作，目前使用 monitor 模块模拟内存映射的 IO 行为以测试 NEMU。

### 使用方法

```
$ make monitor
$ ./monitor.bin monitor/config # 启动模拟外设
$ make go                      # 带参数启动 nemu
```

`config` 文件记录了相关的设定，如屏幕尺寸等，格式如下：

```
屏幕横向分辨率
屏幕纵向分辨率
字体横向宽度
字体纵向宽度
屏幕内存映射使用的文件名
键盘内存映射使用的文件名
```

主要需要修改的就是根据自己的操作环境，调整横向分辨率和纵向分辨率，
因为 monitor, 以及我们的 vga 文字模式硬件控制器，都是使用填充空格来达到换行效果的。
