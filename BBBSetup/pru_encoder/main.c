

#include <stdint.h>

#include <pru_cfg.h>

/*TODO: Include intc.h */

#include <pru_intc.h>

// PRU Core 1

#define PRU1

volatile register uint32_t __R30;

volatile register uint32_t __R31;

/* Defines */

#define PRU0

/* PRU0-to-PRU1 interrupt */

#define PRU0_PRU1_EVT       (16)

#define PRU0_PRU1_TRIGGER   (__R31 = (PRU0_PRU1_EVT - 16) | (1 << 5))

/* SW1 offset */

#define SW1     (1 << 5)

/* INTC configuration

 * We are going to map User event 16 to Host 1

 * PRU1 will then wait for r31 bit 31 (designates Host 1) to go high

 * */

void configIntc(void)

{

    /* Clear any pending PRU-generated events */

    __R31 = 0x00000000;

    /* Map event 16 to channel 1 */

    CT_INTC.CMR4_bit.CH_MAP_16 = 1; /*TODO: Select correct bit field and enter proper value */;

    /* Map channel 1 to host 1 */

    CT_INTC.HMR0_bit.HINT_MAP_1 = 1; /*TODO: Select correct bit field and enter proper value */;

    /* Ensure event 16 is cleared */

    CT_INTC.SICR = 16; /*TODO: Clear proper event */;

    /* Enable event 16 */

    CT_INTC.EISR = 16; /*TODO: Enable proper event */;

    /* Enable Host interrupt 1 */

    CT_INTC.HIEISR |= (1 << 0); /*TODO: Enable proper event */;

    // Globally enable host interrupts

    CT_INTC.GER = 1; /*TODO: Enable global events */;

}

void main(void)

{

    /* Configure GPI and GPO as Mode 0 (Direct Connect) */

    CT_CFG.GPCFG0 = 0x0000;

    /* Clear GPO pins */

    __R30 &= 0xFFFF0000;

    /* Configure INTC */

    configIntc();

    while (1) {

        /* Wait for SW1 to be pressed */

        if ((__R31 & SW1) != SW1) {

            /* Interrupt PRU1, wait 500ms for cheap "debounce" */

            __delay_cycles(100000000);

            /* TODO: Trigger interrupt - see #defines */

            PRU0_PRU1_TRIGGER;

        }

    }

}
