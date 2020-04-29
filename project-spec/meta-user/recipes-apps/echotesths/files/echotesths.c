/*
 * echo_test.c
 *
 *  Created on: Oct 4, 2014
 *      Author: etsam
 */

/*
 * Test application that data integraty of inter processor
 * communication from linux userspace to a remote software
 * context. The application sends chunks of data to the
 * remote processor. The remote side echoes the data back
 * to application which then validates the data returned.
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <linux/rpmsg.h>
union {
unsigned int integer;
char byte[4];
} temp32bitint;


struct _payload {
	unsigned long num;
	unsigned long size;
	char data[];
};
typedef struct
{
	uint32_t lowerfrequency;
	uint32_t upperfrequency;
	uint32_t pingrange;
	uint32_t chirpon0ff;
	uint32_t pingenable;
	uint32_t ping_index;
	uint32_t numofsample;
	char  data[100];

}pingpacket_t;

static int charfd = -1, fd = -1, err_cnt;
extern int sharedmemreadback(void);
extern long currentTimeMillis(void);

struct _payload *i_payload;
struct _payload *r_payload;
pingpacket_t packettosend;
#define RPMSG_GET_KFIFO_SIZE 1
#define RPMSG_GET_AVAIL_DATA_SIZE 2
#define RPMSG_GET_FREE_SPACE 3

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (512 - RPMSG_HEADER_LEN)
#define PAYLOAD_MIN_SIZE	1
#define PAYLOAD_MAX_SIZE	(MAX_RPMSG_BUFF_SIZE - 24)
#define NUM_PAYLOADS		(PAYLOAD_MAX_SIZE/PAYLOAD_MIN_SIZE)

 #define RPMSG_BUS_SYS1 "/sys/bus/rpmsg"

 char fw_dst_path[] = "/lib/firmware/openamp.elf";// "/lib/firmware/echo-test-a9-1.elf";
 char sbuf[512];
 int r5_id = 0;

 /* write a string to an existing and writtable file */
 int file_write(char *path, char *str)
 {
 	int fd;
 	ssize_t bytes_written;
 	size_t str_sz;

 	fd = open(path, O_WRONLY);
 	if (fd == -1) {
 		perror("Error");
 		return -1;
 	}
 	str_sz = strlen(str);
 	bytes_written = write(fd, str, str_sz);
 	if (bytes_written != str_sz) {
 	        if (bytes_written == -1) {
 			perror("Error");
 		}
 		close(fd);
 		return -1;
 	}

 	if (-1 == close(fd)) {
 		perror("Error");
 		return -1;
 	}
 	return 0;
 }

 /* Stop remote CPU and Unload drivers */
 void stop_remote(void)
 {
 	system("modprobe -r rpmsg_char");
 	sprintf(sbuf,
 		"/sys/class/remoteproc/remoteproc%u/state",
 		r5_id);
 	(void)file_write(sbuf, "stop");
 }


int main(int argc, char *argv[])
{
	int ret, i, j;
	int size, bytes_rcvd, bytes_sent;
	err_cnt = 0;
	int opt;
	char *rpmsg_dev="virtio0.rpmsg-openamp-demo-channel.-1.0";
	int ntimes = 1;
	char fpath[256];
	char rpmsg_char_name[16];
	struct rpmsg_endpoint_info eptinfo;
	char ept_dev_name[16];
	char ept_dev_path[32];
	char *user_fw_path = 0;
//	printf("timestamp for ddr started  reading %d\n", (int)time(NULL));
printf("%ld\n", currentTimeMillis());

	while ((opt = getopt(argc, argv, "d:n:")) != -1) {
		switch (opt) {
			case 'f':
			user_fw_path = optarg;
			break;
		case 'd':
			rpmsg_dev = optarg;
			break;
		case 'n':
			ntimes = atoi(optarg);
			break;
		default:
			printf("getopt return unsupported option: -%c\n",opt);
			break;
		}
	}
//	printf("\r\n Echo test start \r\n");
//	printf("\r\nMaster>Loading remote firmware\r\n");
		if (user_fw_path) {
			sprintf(sbuf, "cp %s %s", user_fw_path, fw_dst_path);
			system(sbuf);
		}

		/* Write firmware name to remoteproc sysfs interface */
		sprintf(sbuf,
			"/sys/class/remoteproc/remoteproc%u/firmware",
			r5_id);
		if (0 != file_write(sbuf, "echo-test-a9-1.elf")) {
			return -EINVAL;
		}

		/* Tell remoteproc to load and start remote cpu */
		sprintf(sbuf,
			"/sys/class/remoteproc/remoteproc%u/state",
			r5_id);
		if (0 != file_write(sbuf, "start")) {
			return -EINVAL;
		}

//		printf("\r\nMaster>probe rpmsg_char\r\n");
		ret = system("modprobe rpmsg_char");
		if (ret < 0) {
			perror("Failed to load rpmsg_char driver.\n");
			ret = -EINVAL;
			goto error0;
		}


//	time_t mytime;
//			mytime = time(NULL);
//			printf(ctime(&mytime));e
//	printf("\r\n Open rpmsg dev %s! \r\n", rpmsg_dev);
	sprintf(fpath, "%s/devices/%s", RPMSG_BUS_SYS1, rpmsg_dev);
	if (access(fpath, F_OK)) {
		fprintf(stderr, "Not able to access rpmsg device %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	ret = bind_rpmsg_chrdev(rpmsg_dev);
	if (ret < 0)
		return ret;
	charfd = get_rpmsg_chrdev_fd(rpmsg_dev, rpmsg_char_name);
	if (charfd < 0)
		return charfd;

	/* Create endpoint from rpmsg char driver */
	strcpy(eptinfo.name, "rpmsg-openamp-demo-channel");
	eptinfo.src = 0;
	eptinfo.dst = 0xFFFFFFFF;
	ret = rpmsg_create_ept(charfd, &eptinfo);
	if (ret) {
		printf("failed to create RPMsg endpoint.\n");
		return -EINVAL;
	}
	if (!get_rpmsg_ept_dev_name(rpmsg_char_name, eptinfo.name,
				    ept_dev_name))
		return -EINVAL;
	sprintf(ept_dev_path, "/dev/%s", ept_dev_name);
	fd = open(ept_dev_path, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror("Failed to open rpmsg device.");
		close(charfd);
		return -1;
	}

	i_payload = (struct _payload *)malloc(2 * sizeof(unsigned long) + PAYLOAD_MAX_SIZE);
	r_payload = (struct _payload *)malloc(2 * sizeof(unsigned long) + PAYLOAD_MAX_SIZE);

	if (i_payload == 0 || r_payload == 0) {
		printf("ERROR: Failed to allocate memory for payload.\n");
		return -1;
	}
int sendingvalues;
i_payload->data[0]= 0x40;
i_payload->data[1]= 0x06;
i_payload->data[2]= 0;
i_payload->data[3]= 0;
i_payload->data[4]= 0x20;
i_payload->data[5]= 0x07;
i_payload->data[6]= 0;
i_payload->data[7]= 0;
i_payload->data[8]= 0x80;
i_payload->data[9]= 0x0;
i_payload->data[10]= 0x0;
i_payload->data[11]= 0x0;
i_payload->data[12]= 0x5;//5*10 samples = 50 samples should be returned by the cor
i_payload->data[13]= 0x0;
i_payload->data[14]= 0x0;
i_payload->data[15]= 0x0;
i_payload->data[16]= 0x0;
i_payload->data[17]= 0x0;
i_payload->data[18]= 0x0;
i_payload->data[19]=  0x0;
i_payload->data[20]= 0x0;
i_payload->data[21]= 0x0;
i_payload->data[22]= 0x0;
i_payload->data[23]= 0x0;


for(  sendingvalues = 0 ; sendingvalues <24; sendingvalues++)
{
printf(" values sent %x\r\n", i_payload->data[sendingvalues]);

}
			int k;

			i_payload->num = 1;
			size = 24;
			i_payload->size = size;

			/* Mark the data buffer. */
	//		memset(&(i_payload->data[0]), 0xA5, size);

			printf("\r\n sending payload number");
			printf(" %ld of size %d\r\n", i_payload->num,
			(2 * sizeof(unsigned long)) + size);

			bytes_sent = write(fd, i_payload,
			(2 * sizeof(unsigned long)) + size);

			if (bytes_sent <= 0) {
				printf("\r\n Error sending data");
				printf(" .. \r\n");
				return -1;;
			}
			printf("echo test: sent : %d\n", bytes_sent);

			r_payload->num = 0;
			bytes_rcvd = read(fd, r_payload,128);
				//	(2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE);
			while (bytes_rcvd <= 0) {
				usleep(10000);
				bytes_rcvd = read(fd, r_payload,
					(2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE);
			}
			printf(" received payload number ");
			printf("%ld of size %d\r\n", r_payload->num, bytes_rcvd);

			/* Validate data buffer integrity. */
			for (k = 0; k < bytes_rcvd; k++) {

					printf(" data at index %d\r\n is %x \r\n",k ,r_payload->data[k]);

			}
			bytes_rcvd = read(fd, r_payload,
			(2 * sizeof(unsigned long)) + PAYLOAD_MAX_SIZE);



	free(i_payload);
	free(r_payload);

	close(fd);
	if (charfd >= 0)
		close(charfd);
		ret= 	sharedmemreadback();

		error0:
			stop_remote();
//	ret= 	sharedmemreadback();
	return 0;
}
