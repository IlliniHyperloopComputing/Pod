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

static void do_msg(int fd, int len)
{
	struct spi_ioc_transfer	xfer[2];
	unsigned char		buf[32], *bp;
	int			status;

	memset(xfer, 0, sizeof xfer);
	memset(buf, 0, sizeof buf);

	if (len > sizeof buf)
		len = sizeof buf;

	buf[0] = 0xaa;
	xfer[0].tx_buf = (unsigned long)buf;
	xfer[0].len = 1;

	xfer[1].rx_buf = (unsigned long) buf;
	xfer[1].len = len;

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return;
	}

	printf("response(%2d, %2d): ", len, status);
	for (bp = buf; len; len--)
		printf(" %02x", *bp++);
	printf("\n");
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
	const char	*name;

	fd = open("/dev/spidev1.0", O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	uint32_t speed = 1000000;
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
	
	uint16_t tx = 0xDE;	
	uint16_t tx1 = 0xAD;
	uint16_t rx = 0xBEEF;	
	struct timespec ts, ts2;
	while(getc(stdin)){
		clock_gettime(CLOCK_MONOTONIC,&ts);
		write(fd,&tx,1);
		clock_gettime(CLOCK_MONOTONIC, &ts2);
		write(fd,&tx1,1);
		if(rx == 0x88){
			printf("Omg it works\n");
		}
		double final = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
		printf("read: %x\n",rx);
		printf("took %lf seconds\n", final);
	}

	close(fd);
	return 0;
}
