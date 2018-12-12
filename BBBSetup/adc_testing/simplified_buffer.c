#define _GNU_SOURCE

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <linux/types.h>
#include <string.h>
#include <poll.h>
#include <endian.h>
#include <getopt.h>
#include <inttypes.h>
#include <time.h>
#include "iio_utils.h"

/**
 * process_scan() - print out the values in SI units
 * @data:		pointer to the start of the scan
 * @channels:		information about the channels. Note
 *  size_from_channelarray must have been called first to fill the
 *  location offsets.
 * @num_channels:	number of channels
 **/
void process_scan(char *data, int num_channels)
{
	uint16_t * arr = (uint16_t *)data;
	printf("arr[0]: %d  arr[1]: %d arr[2]:%d\n", arr[0], arr[1], arr[2]);

}

int main(int argc, char **argv)
{
	unsigned long num_loops = 2;
	unsigned long timedelay = 1000000;
	unsigned long buf_len = 128;
	int ret, c, i, j, toread;
	int fp;

	int num_channels;
	char *trigger_name = NULL, *device_name = NULL;
	char *dev_dir_name, *buf_dir_name;

	char *data;
	ssize_t read_size;
	int trig_num;
	char *buffer_access;
	int scan_size;
	int noevents = 0;
	char *dummy;

	struct iio_channel_info *channels;

	while ((c = getopt(argc, argv, "l:w:c:et:n:")) != -1) {
		switch (c) {
		case 'n':
			device_name = optarg;
			break;
		case 'e':
			noevents = 1;
			break;
		case 'c':
			num_loops = strtoul(optarg, &dummy, 10);
			break;
		case 'w':
			timedelay = strtoul(optarg, &dummy, 10);
			break;
		case 'l':
			buf_len = strtoul(optarg, &dummy, 10);
			break;
		case '?':
			return -1;
		}
	}

	dev_dir_name = "/sys/bus/iio/devices/iio:device0";
	buf_dir_name = "/sys/bus/iio/devices/iio:device0/buffer";
	buffer_access = "/dev/iio:device0";

	/*
	 * Parse the files in scan_elements to identify what channels are
	 * present
	 */
	ret = build_channel_array(dev_dir_name, &channels, &num_channels);
	if (ret) {
		perror("Problem reading scan element information: \n");
		return ret;
	}
	printf("Num chamnnels: %d\n", num_channels);

	scan_size = num_channels * 2;

	/* Setup ring buffer parameters */
	ret = write_sysfs_int("length", buf_dir_name, buf_len);
	if (ret < 0){
		perror("Failed to open 'device0/buffer/length':");
		return ret;
	}
	printf("Wrote buffer length\n");

	// Disable the buffer first
	ret = write_sysfs_int("enable", buf_dir_name, 0);
	usleep(250000);//wait a quarter of second

	/* Enable the buffer */
	ret = write_sysfs_int("enable", buf_dir_name, 1);
	if (ret < 0){
		perror("Failed to open 'device0/buffer/enable':");
		return ret;
	}
	printf("Wrote enable bit\n");

	data = malloc(scan_size*buf_len);

	/* Attempt to open non blocking the access dev */
	fp = open(buffer_access, O_RDONLY|O_NONBLOCK);
	if (fp == -1) { /* If it isn't there make the node */
		ret = -errno;
		perror("Failed to open '/dev/device0' :");
		return ret;
	}
	printf("Opened up /dev access \n");

	usleep(1000000);
	for (j = 0; j < num_loops; j++) {
		usleep(timedelay);
		read_size = read(fp, data, buf_len*scan_size);
		if (read_size == -1)
			perror("READ:");
		if (read_size == -EAGAIN) {
			printf("nothing available\n");
			continue;
		}
		for (i = 0; i < read_size/scan_size; i++)
			 process_scan(data + scan_size*i, num_channels);
	}

	/* Stop the buffer */
	ret = write_sysfs_int("enable", buf_dir_name, 0);

	close(fp);
	free(data);
	return ret;
}
