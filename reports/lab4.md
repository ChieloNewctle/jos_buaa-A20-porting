# 目标效果

在不暴露任何内核的内存情况下，`fktest` 运行正常，可以进行 `fork`，
无进程后可以正常地在 `svc` 模式下继续运行。

同时完成 `ipc` 通信，
并且 `pingpong` 测试程序运行正常。


# Copy on Write 妥协

由于对页表进行重新设计较为困难，所以不实现 copy on write。

对于 `fork` 直接复制所有页，并只在内核态进行复制，内核的信息不会通过内存映射提供给用户。


# 上下文切换的问题

测试很久后发现 `ttb` 切换后仍需要对 `tlb` 进行 `invalidate`，并切换前将 `data cache` 清理，
否则会有严重的内存不一致的问题，导致复制的进程执行时，栈获取的值与分配的页中存储的不一致。


# IPC

由于不希望暴露内核的内存，因此 `env` 用户程序是无法得到的，
所以需要一些特殊的实现来保证用户进程可以正常地得到所有返回值。

由于 `ipc_recv` 时，进程处于 `NOT_RUNNABLE`，这时可以看做进程的所有信息全部冻结。
对于传入的参数 `whom` 和 `perm`，保存后在 `ipc_send` 时，不由用户进程处理，
由系统调用来对参数指向的地址进行写入，来完成所有信息的返回。

