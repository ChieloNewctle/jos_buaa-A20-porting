# 内核虚拟地址空间

由于要求尽可能地保留原有的代码，需要保持 2G-2G 模式，
但是在启动时，只有物理地址可以使用，
此时需要首先建立一个简单的段表来将一部分内核虚拟地址映射到对应的物理地址上。


# `mkimage`

需要制作 u-boot 载入的内核镜像，`mkimage` 来自 [BPI-bsp](https://github.com/BPI-SINOVOIP/BPI-bsp) 的 u-boot。
