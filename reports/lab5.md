# 目标效果

文件系统实现基本正确。
`fs/serv.c` 自检正常，`user/fstest.c` 正常执行。


# 硬件接口妥协

硬件接口过于复杂，还是难以在短时间内完成完整的硬件接口；
其次，即使实现了 eMMC 的接口，SD卡上还有分区表，并且两个分区分别为 vfat 和 ext4。
而 Lab5 提供的文件系统的格式是自定义的，因此再重新更改还是有些复杂。

最终使用 `bintoc` 来将镜像转为 C 的代码，并编译到内核中，由系统调用控制读写。


# FsFormat

`tools` 中的 `fsformat` 是 Lab5 提供的部分并加以修改。
其中一个逻辑上的变更是不再 `reverse` 每个字 (Word)。
因为目标机器 (ARM) 是小端的，开发环境 x86_64 也是小端的，所以不需要再 `reverse`。


# Dirty

Lab5 很好地利用了虚拟机 MIPS 的 MMU 的特性，可以直接读取脏权限位。
但是 ARMv7 的 MMU 没有这个特性，所以需要手动实现文件系统的 Dirty 相关的记录。

