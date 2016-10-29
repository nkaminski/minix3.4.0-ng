# minix3.4.0-ng

MINIX 3.4.0 modified with multicast IPC for our CS551 class.

qemu-system-i386 --enable-kvm -m 1024 -hda /PATH/TO/minix_x86.img -netdev user,id=user.0 -device virtio-net,netdev=user.0 -curses
