

#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_iep.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include <resource_table_1.h>

// PRU Core 1

#define PRU1

volatile register uint32_t __R30;

volatile register uint32_t __R31;


/* Host-1 Interrupt sets bit 31 in register R31 */
#define HOST_INT            ((uint32_t) 1 << 31)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST     18
#define FROM_ARM_HOST   19

/*
 * Using the name 'rpmsg-client-sample' will probe the RPMsg sample driver
 * found at linux-x.y.z/samples/rpmsg/rpmsg_client_sample.c
 *
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
//#define CHAN_NAME           "rpmsg-client-sample"
#define CHAN_NAME         "rpmsg-pru"

#define CHAN_DESC           "Channel 31"
#define CHAN_PORT           31

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK   4


/* Defines */

#define ENC0    (1 << 8)  // P8_27
#define ENC1    (1 << 10) // P8_28
#define ENC2    (1 << 9)  // P8_29
#define ENC3    (1 << 6)  // P8_39
#define ENC4    (1 << 7)  // P8_40
#define ENC5    (1 << 4)  // P8_41
#define ENC6    (1 << 5)  // P8_42
#define ENC7    (1 << 2)  // P8_43
#define ENC8    (1 << 3)  // P8_44
#define ENC9    (1 << 0)  // P8_45
#define ENC10   (1 << 1)  // P8_46

// array of masks so I can easily loop through them
uint32_t masks[11] = {ENC0, ENC1, ENC2, ENC3, ENC4, ENC5, ENC6, ENC7, ENC8, ENC9, ENC10};

uint32_t htime_ctr = 0;

uint32_t counts[11] = {0}; //Number of times we have seen the signal
uint32_t htime[11] = {0}; //upper 32 bits of last time there was a high or low signal
uint32_t ltime[11] = {0}; //lower 32 bits of last time there was a high or low signal

uint32_t hlasthigh[11] = {0};
uint32_t llasthigh[11] = {0}; // These are what the time deltas are based off of.

uint32_t hdelta[11] = {0}; //time between the most recently seen signal and the signal before that one.
uint32_t ldelta[11] = {0};

uint8_t isHigh[11] = {0}; //Signal is currently high, need it to go low.

uint8_t payload[RPMSG_BUF_SIZE];

#include "optical.h"

void init_rpmsg(void)
{


}

void main(void)

{

    struct pru_rpmsg_transport transport;
    uint16_t src, dst, len;
    volatile uint8_t *status;

    /* Allow OCP master port access by the PRU so the PRU can read external memories */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    /* Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

    /* Make sure the Linux drivers are ready for RPMsg communication */
    status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

    /* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);


    /* Configure GPI and GPO as Mode 0 (Direct Connect) */
    CT_CFG.GPCFG0 = 0x0000;
    /* Clear GPO pins */
    __R30 &= 0xFFFF0000;
    /* Clear any pending PRU-generated events */
    __R31 = 0x00000000;

    CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0x0; // disable counter
    CT_IEP.TMR_GLB_CFG_bit.DEFAULT_INC = 0x1; // set increment value to 1
    CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 0x1; // Write 1 to clear overflow
    CT_IEP.TMR_COMPEN_bit.COMPEN_CNT = 0x0; // Disable compensation
    CT_IEP.TMR_CNT = 0x1; // Write 1 to clear counter
    CT_IEP.TMR_CMP_CFG_bit.CMP0_RST_CNT_EN = 0x0; // Disable reset of counter if CMP0 event occurs
    CT_IEP.TMR_CMP_CFG_bit.CMP_EN = 0x0; // Disable all CMP[n] events
    CT_IEP.TMR_GLB_CFG_bit.CNT_EN = 0x1; // enable counter


    int i, j;
    while (1) {
        for(i = 0; i<11; i++){
            debounce(masks[i], i);
        }

        /* Check bit 30 of register R31 to see if the ARM has kicked us */
        if (__R31 & HOST_INT) {
            /* Clear the event status */
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            /* Receive all available messages, multiple messages can be sent per kick */
            while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
                /* Echo the message back to the same address from which we just received */
                pru_rpmsg_send(&transport, dst, src, (uint8_t*)&htime_ctr, sizeof(htime_ctr));
            }
        }

        /*if(j % 100 == 0){
            len = 3;
            pru_rpmsg_send(&transport, dst, src, payload, len);
        }
        j++;
        */
    }

}


