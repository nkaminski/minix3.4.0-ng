# minix3.4.0-ng

MINIX 3.4.0 modified with multicast IPC for our CS551 class.

Building from source: 
1.Download source tarball from https://iit.kaminski.io/cs551_proj2/ 
2.Untar source tarball: tar xvf <source>.tar.gz 
3.Move into directory: cd <source> 
4.Build image: ./releasetools/x86_hdimage.sh 
5.Run: qemu-system-i386 --enable-kvm -m 1024 -hda ./minix_x86.img -netdev
user,id=user.0 -device virtio-net,netdev=user.0 -curses


Running from OVA: 
1.Download OVA image from https://iit.kaminski.io/cs551_proj2/ 
2.Open OVA image in VirtualBox


Tests: cd /share/misc 
Note: “( )” designate a range of numbers that is appended to the name 
	sh ./test(0-4).sh 
	sh ./killtest(0-1).sh
	
	
C Program Tests: (In PATH) 
	kill_test_(0-5) 
	no_group_test_0 
	openwait_test_0 
	receive_test_(0-1) 
	recover_test_0 
	send_test_(0-1)
