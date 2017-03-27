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
	dumpstat("/dev/spidev1.0", fd);

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
	dumpstat("/dev/spidev1.1", fd2);
	


	uint16_t tx = 0xde;	
	uint16_t rx = 0xEF;	
	size_t tx_4 = 0xdededede;
	size_t rx_4 ;
	struct timespec ts, ts2;
	int ct = 0;
	double sum = 0;
	uint8_t *rx_buff = malloc(sizeof(uint8_t) * 15);
	uint8_t *rx_buff1 = malloc(sizeof(uint8_t) * 15);
	
	uint16_t crc_check = 0;
	uint16_t crc_check1 = 0;
	int j = 0;
	int k = 0;
	int correct = 0;
	int correct1 = 0;
	int timeout = 10;
	int timeout_ct = 0;
	int num_fails = 0;
	int num_fails1 = 0;
	int num_wins = 0;
	int num_wins1 = 0;
	uint16_t crc = 0; 
	uint16_t crc1 = 0;
	
	
	int loops = 10000;
	while(j<loops){
		memset(rx_buff, 0, 15);
		memset(rx_buff1, 0, 15);
		correct = 0;
		correct1 = 0;
		timeout_ct = 0;
		crc = 0;
		crc1 = 0;

		clock_gettime(CLOCK_MONOTONIC,&ts);
		while((correct == 0 || correct1 == 0) && timeout_ct < timeout){
			crc = 0;
			crc1 = 0;
			tx = 0xAA;
			write(fd, &tx,1);
			write(fd2, &tx,1);
			for(k = 0; k<12; k++){
				read(fd, rx_buff+k, 1);
				crc = CRCCCITT_byte(crc, rx_buff[k]);
				read(fd2, rx_buff1+k, 1);
				crc1 = CRCCCITT_byte(crc1, rx_buff1[k]);
				//printf("fd1 r: %x\n",rx_buff[k]);
			}
			for(k = 12; k<14; k++){
				read(fd, rx_buff+k, 1);
				read(fd2, rx_buff1+k, 1);
				//printf("fd1 r: %x\n",rx_buff[k]);
			}



			crc_check = crc;
			crc_check1 = crc1;
			//crc_check =CRCCCITT(rx_buff,12,0);
			///crc_check1= CRCCCITT(rx_buff1,12,0);
			//printf("crc: %x,  crc_check: %x\n", crc, crc_check);
			
			//for(k = 0; k<14; k++){
			//	printf("fd1 r: %x\n",rx_buff[k]);
			//}
			uint16_t correct_crc = (rx_buff[13]<<8) | (rx_buff[12]);
			uint16_t correct_crc1 = (rx_buff1[13]<<8) | (rx_buff1[12]);

			//printf("correct crc: %x, crc: %x \n", correct_crc, crc);
			if((correct_crc == crc_check) && correct_crc != 0 && !correct){
				correct = 1;
				//num_wins++;
			}
			//else if(!correct){
			//	num_fails ++;
			//	
			//}
			if((correct_crc1 == crc_check1) && correct_crc1 != 0 && !correct1){
				correct1 = 1;
				//num_wins1++;
			}
			//else if(!correct1){
			//	num_fails1++;

			//}
			timeout_ct++;
		}
		clock_gettime(CLOCK_MONOTONIC, &ts2);
		//printf("correct: %d\t correct1: %d\tfailure:%d \n", correct, correct1, timeout_ct);
		if(timeout_ct == timeout){
			printf("10 timeouts\n");
		}
		sum += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
		//printf("took: %lf seconds\n", sum);

		//sum = 0;
		//ct++;
		//double final = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
		j++;
	}
	printf("took: %lf seconds\n", sum);
	printf(" avg: %lf seconds\n", sum/loops);
	printf(" num wins: %d    %d\n", num_wins, num_wins1);
	printf(" num fails: %d   %d\n", num_fails, num_fails1);
	printf(" avg fails: %d\n", num_fails/loops);

	free(rx_buff);
	free(rx_buff1);
	close(fd);
	close(fd2);
	return 0;
}
