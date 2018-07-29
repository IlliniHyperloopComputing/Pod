

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_iep.h>

// PRU Core 1

#define PRU1

volatile register uint32_t __R30;

volatile register uint32_t __R31;

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


#include "optical.h"

void main(void)

{

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



    while (1) {
        int i;
        for(i = 0; i<11; i++){
            debounce(masks[i], i);
        }


        if ((__R31 & ENC1) == ENC1) {


            /* Interrupt PRU1, wait 500ms for cheap "debounce" */

            __delay_cycles(100000000);

        }

    }

}


