#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "crc.h"

static int verbose;

static void do_read(int fd, int len)
{
	unsigned char	buf[32], *bp;
	int		status;

	/* read at least 2 bytes, no more than 32 */
	if (len < 2)
		len = 2;
	else if (len > sizeof(buf))
		len = sizeof(buf);
	memset(buf, 0, sizeof buf);

	status = read(fd, buf, len);
	if (status < 0) {
		perror("read");
		return;
	}
	if (status != len) {
		fprintf(stderr, "short read\n");
		return;
	}

	printf("read(%2d, %2d): %02x %02x,", len, status,
		buf[0], buf[1]);
	status -= 2;
	bp = buf + 2;
	while (status-- > 0)
		printf(" %02x", *bp++);
	printf("\n");
}

uint8_t trade_byte(int fd, uint8_t msg)
{
	struct spi_ioc_transfer	xfer[2];
	uint8_t in;

	memset(xfer, 0, sizeof(xfer));

	xfer[0].tx_buf =  (size_t)(&msg);
	xfer[0].len = 1;
	xfer[0].cs_change = 1;

	xfer[1].rx_buf = (size_t)(&in);
	xfer[1].len = 1;
	xfer[1].cs_change = 0;

	

	int status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE"); return;
	}

	return in;

}

static void dumpstat(const char *name, int fd)
{
	__u8	mode, lsb, bits;
	__u32	speed;

	if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
		perror("SPI rd_mode");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
		perror("SPI rd_lsb_fist");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
		perror("SPI bits_per_word");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
		perror("SPI max_speed_hz");
		return;
	}

	printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",
		name, mode, bits, lsb ? "(lsb first) " : "", speed);
}

int main(int argc, char **argv)
{
	int		c;
	int		readcount = 0;
	int		msglen = 0;
	int		fd;
	int		fd2;
	const char	*name;

	fd = open("/dev/spidev1.0", O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	uint32_t speed = 500000;
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		perror("SPI max_speed_hz");
		return;
	}


	uint8_t bits_per_word = 8;
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
		perror("SPI bits_per_word");
		return;
	}
	dumpstat(name, fd);

	fd2 = open("/dev/spidev1.1", O_RDWR);
	if (fd2 < 0) {
		perror("open");
		return 1;
	}
	if (ioctl(fd2, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		perror("SPI max_speed_hz");
		return;
	}
	if (ioctl(fd2, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
		perror("SPI bits_per_word");
		return;
	}
	dumpstat(name, fd2);
	


	uint16_t tx = 0xde;	
	uint16_t rx = 0xEF;	
	size_t tx_4 = 0xdededede;
	size_t rx_4 ;
	struct timespec ts, ts2;
	int ct = 0;
	double sum = 0;
	uint8_t rx_buff[15];

	uint8_t to_crc = 0xde;
	uint16_t crc_check = CRCCCITT(&to_crc,1,0x0000);
	uint16_t crc_check1 = CRCCCITT_byte(0,0x00de);

	printf("crc_check: %x    %x\n",crc_check, crc_check1); 

	while(getc(stdin)){
		memset(rx_buff, 0, 15);
		crc_check = 0;

		clock_gettime(CLOCK_MONOTONIC,&ts);
		//write(fd,&tx,1);
		//read(fd,&rx,1);
		//printf("fd1 r: %x\n",rx);

		tx = 0xAA;
		write(fd, &tx,1);
		int k = 0;
		for(k = 0; k<14; k++){
			read(fd, rx_buff+k, 1);
			//printf("fd1 r: %x\n",rx_buff[k]);
		}


		clock_gettime(CLOCK_MONOTONIC, &ts2);
		sum = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
		printf("took: %lf seconds\n", sum);

		crc_check = CRCCCITT(rx_buff,12,0);
		printf("CRC: b-y-b:%x    %x\n", crc_check);
		
		for(k = 0; k<14; k++){
			printf("fd1 r: %x\n",rx_buff[k]);
		}
		uint16_t correct_crc = (rx_buff[13]<<8) | (rx_buff[12]);

		printf("correct crc: %x \n", correct_crc);
		if(correct_crc == crc_check){
			printf("YAY ERROR CHECKING worked\n");
		}
		//sum = 0;
		//ct++;
		//double final = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
	}
	//double final = (sum)///1000000000.0;
	//printf("took: %lf seconds\n", sum);
	//printf(" avg: %lf seconds\n", sum/10000);

	close(fd);
	close(fd2);
	return 0;
}
