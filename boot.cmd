setenv bootm_boot_mode sec
load mmc 0:1 0x40000000 uImage.my
bootm 0x40000000
