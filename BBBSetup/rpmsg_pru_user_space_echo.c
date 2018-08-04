/*
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>

#define MAX_BUFFER_SIZE		512
char readBuf[MAX_BUFFER_SIZE];

#define NUM_MESSAGES		20
#define DEVICE_NAME		"/dev/rpmsg_pru31"

typedef struct pru_data{
	uint32_t counts[11];
	uint32_t decays[11];
	uint32_t deltas[11];
} pru_data;

int main(void)
{
	struct pollfd pollfds[1];
	int i;
	int result = 0;

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

	/*
	 * If the RPMsg channel doesn't exist yet the character device
	 * won't either.
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted.
	 */
	if (pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	printf("Opened %s\n\n", DEVICE_NAME);

	result = write(pollfds[0].fd, "start", 6);
	/* Poll until we receive a message from the PRU and then print it */
	result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);

	struct timespec tm1;
	struct timespec tm2;

	double avg_time = 0;

	for (i = 0; i < NUM_MESSAGES; i++) {
		/* Send 'hello world!' to the PRU through the RPMsg channel */

		printf("type anything to continue\n");
		char c= getchar();
		
		clock_gettime(CLOCK_MONOTONIC, &tm1);
		result = write(pollfds[0].fd, "n", 2);
		if (result == 0)
			printf("Message NOT Sent to PRU");

		/* Poll until we receive a message from the PRU and then print it */
		result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
		clock_gettime(CLOCK_MONOTONIC, &tm2);
		if (result > 0){
			printf("got message with this many bytes: %d\n", result);
			pru_data prud;
			memcpy(prud.counts, readBuf, 44);
			memcpy(prud.decays, readBuf+44, 44);
			memcpy(prud.deltas, readBuf+88, 44);
			for(int j = 0; j < 11; j++){
				printf("counts[%d] = %lu  ", j, prud.counts[j]);
				printf("decays[%d] = %f  ", j , prud.decays[j]/ (4294967295.0) *21.474836475);
				printf("deltas[%d] = %lu  ", j, prud.deltas[j]);
			}
			double timer = (((double)tm2.tv_sec-tm1.tv_sec)*1E9 + ((double)tm2.tv_nsec - tm1.tv_nsec))/1E9;
			avg_time += timer;
			printf("\ntook this long: %f \n \n", timer);
		}
	}
	printf("avg time: %f\n", avg_time/NUM_MESSAGES);

	/* Received all the messages the example is complete */
	printf("Received %d messages, closing %s\n", NUM_MESSAGES, DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}

