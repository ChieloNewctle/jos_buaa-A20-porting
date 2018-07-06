# Porting JOS (modified by BUAA) to BananaPi-M1 (Allwinner A20)

This is a repository for porting JOS directed by
[BUAA OS Guide Book](https://git.cscore.net.cn/star_os/guide-book) to
[Allwinner A20 SoC](http://linux-sunxi.org/A20).

Require the cross compiler for ARM configurated in `include.mk`.
Use u-boot to load kernel image (`uImage.my`) with startup script (`boot.src`) in an SD card.
