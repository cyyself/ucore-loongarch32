# uCore-LoongArch 32

本项目基于[ucore-thumips](https://github.com/z4yx/ucore-thumips)，将其移植到了LoongArch 32上。并成功跑通了所有的用户进程。

我们同样移植了去除答案的实验教学版本与实验指导书，实验版本位于本项目的no_answer分支，实验指导书[在此](https://cyyself.github.io/ucore_la32_docs/)。

我们已经过测试，本移植版本可以在[chiplab](https://gitee.com/loongson-edu/chiplab/)的loongarch32软核上运行。

## 编译环境准备

我们已经将实验环境打包为Docker镜像，并发布到Docker Hub，可以使用`docker pull chenyy/la32r-env`进行安装，并在该容器内完成编译即可。

## Makefile操作

### 编译内核

```shell
make
```

### 在QEMU运行内核

```shell
make qemu
```

### 调试内核

在一个终端执行以下内容，它将以debug模式启动qemu，并监听在1234端口：

```shell
make debug
```

在另一个终端启动gdb：

```shell
make gdb
```

## 效果展示

![debug](doc/img/debug.png)

## LA32与MIPS相比的一些较大改动说明

### 0. 跳转等指令不再需要软件延迟槽

这一点对于操作系统而言影响并不是很大，但是对于处理器核设计是非常重要的一点。

### 1. CP0与CSR

在LoongArch32中，CSR寄存器的作用相当于MIPS的CP0寄存器。

这里需要注意，`csrwr rd, csr_num`指令是将rd寄存器与csr_num进行对调，而非MIPS时代的`mtc0`只是单纯将值写入CP0寄存器。

### 2. 例外处理方式

在MIPS中，我们通过CP0的`Status`寄存器的`EXL`位来表示正在处理异常，因此当EXL为1时强制为内核态。

而LoongArch32的CSR中，取消了`Status`类似的寄存器，而分为了`PRMD`与`CRMD`。表示例外前状态和当前状态，而使用`ertn`指令（类似MIPS的`eret`）则会自动将`PRMD`还原到`CRMD`。这一点上比MIPS更加科学。

### 3. 存储空间管理

在MIPS架构中，我们熟悉的固定地址空间`kseg0`和`kseg1`等被舍弃。在LoongArch32中，被改为了通过CSR中的DMW寄存器配置地址映射窗口。这一部分代码位于`kern/init/entry.S`。考虑到以后可能需要在chiplab上使用PMON进行载入，因此我们按照了PMON相同的方式映射了一段内核的内存地址，也就是`0xa0000000-0xbfffffff`直接映射到`0x00000000-0x1fffffff`。而对于IO设备，以及载入用户进程需要进行的Uncached操作，配置了`0x80000000-0x9fffffff`直接映射到`0x00000000-0x1fffffff`。

对MIPS了解的同学不难看出，我们采用的配置方式非常类似于MIPS的KSEG0与KSEG1，但是对地址进行了对调。

### 4. 通用寄存器

LoongArch32中有一个比较特殊的地方，`v0`与`a0`，`v1`与`a1`寄存器尽管别名不同，但使用了相同的地址。这就导致uCore的MIPS版本原有的一些设计与它存在冲突，例如系统调用我们需要通过`a`寄存器传递参数，然后通过`v0`寄存器拿回系统的返回值。而对于一个特殊的系统调用`SYS_exec`需要通过`a`寄存器传递参数，如果我们把它的返回值直接写入`v0`会造成参数被覆盖。而由于LoongArch32中设计了8个`a`寄存器，因此这里直接将`a7`作为系统调用返回值的存储寄存器。

### 5. 系统调用

尽管LoongArch32的`syscall`指令含有15位立即数，但是在文档中并没有找到例外处理时如何读取这个系统调用号，因此这里将这一部分立即数全部设置为0。并使用`a7`寄存器传递系统调用号。

## 真实硬件运行

如果硬件未保证I-D Cache一致性，需要正确实现CACOP指令，并修改`kern/include/loongarch.h`中的缓存行大小宏定义。