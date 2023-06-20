// Sample commands: 
// gcc secondary_vm.c -o secondary_vm
// ./secondary_vm -c 2 -m 4 -d secVM.img -t raw -e 1 create
#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>

int main(int argc, char *argv[])
{
	const char* HELP = "Usage: ./secondary_vm [options] <operations> \n \
				Options: \n \
				\t -i secondary VMID \n \
				\t -c number of CPUs \n \
				\t -m memory size (GB) \n \
				\t -d disk image \n \
				\t -t disk type \n \
				\t -e vm encryption status: 1 - encrypted; 0 - unencrypted \n \
				Operations: \n \
				\t help \n \
				\t create \n \
				\t destroy \n \
				\t list \n \
				\t console \n";

	if (argc > 14 || argc <= 1 || !strcmp(argv[1],"help")) {
		printf("%s", HELP);
		return 1;
	}
	char all_argv[256];
	char disk_name[64];
	strcpy(all_argv, argv[1]);
	int flag = 0;
	for (int i = 2; i < argc; i++) {
		strncat(all_argv, " ", 1);
		strncat(all_argv, argv[i], strlen(argv[i]));
		if (!strcmp(argv[i], "create")) {
			flag = 1;	
		}
		else if (!strcmp(argv[i], "-d")) {
			flag = 2;
		}
		else if (flag == 2) {
			strcpy(disk_name, argv[i]);
			flag = 3;
		}
	}

	if (flag) {
		system("umount /mnt");
	}

	int s = socket(AF_VSOCK, SOCK_STREAM, 0);

	struct sockaddr_vm addr;
	memset(&addr, 0, sizeof(struct sockaddr_vm));
	addr.svm_family = AF_VSOCK;
	addr.svm_port = 9999;
	addr.svm_cid = VMADDR_CID_HOST;

	connect(s, &addr, sizeof(struct sockaddr_vm));

	/*
	for (int i = 1; i < argc; i++) {
		send(s, argv[i], strlen(argv[i]), 0);
		send(s, " ", 1, 0);
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	*/
	printf("Before Send: %s\n", all_argv);
	send(s, all_argv, strlen(all_argv), 0);

	char buf[512];
	size_t msg_len;
	msg_len = recv(s, &buf, 512, 0);
	printf("Received %lu bytes: %.*s\n", msg_len, msg_len, buf);
	system("mount /dev/vdb/ /mnt");
	
	close(s);

	return 0;
}
