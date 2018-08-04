/*
 * optical.h
 *
 *  Created on: Jun 24, 2018
 *      Author: rgw3d
 */

#ifndef OPTICAL_H_
#define OPTICAL_H_

#define TWO_THOUSAND_HZ 100000


extern uint32_t htime_ctr;

extern uint32_t counts[11]; //Number of times we have seen the signal
extern uint32_t htime[11]; //upper 32 bits of last time there was a high or low signal
extern uint32_t ltime[11]; //lower 32 bits of last time there was a high or low signal

extern uint32_t hdecay[11];
extern uint32_t ldecay[11]; // These are what the time deltas are based off of.

extern uint32_t hdelta[11]; //time between the most recently seen signal and the signal before that one.
extern uint32_t ldelta[11];

extern uint8_t isHigh[11]; //Signal is currently high, need it to go low.

uint32_t htime_diff = 0;
uint32_t ltime_diff = 0;
uint32_t cur_time = 0;

/*
#define diff_time(htime_diff, ltime_diff, htime, ltime) {\
    htime_diff = htime_ctr - htime;\
    ltime_diff = CT_IEP.TMR_CNT - ltime;\
    int overflow = CT_IEP.TMR_GLB_STS_bit.CNT_OVF;\
    if(overflow != 0){\
        htime_diff ++;\
        ltime_diff = UINT32_MAX - ltime_diff;\
        CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 1;\
        htime_ctr ++;\
    }\
}
*/

//htime_ctr - htime > 1  then set max
//htime_ctr - htime == 1 and curtime >
//#define diff_time_new(htime_diff, ltime_diff, htime, ltime) {\

//}



inline
void debounce(const uint32_t mask, const uint32_t i) {

    // Grab the current time
    cur_time = CT_IEP.TMR_CNT;
    hdecay[i] = htime_ctr - htime[i];

    // Check if there was an overflow
    if(CT_IEP.TMR_GLB_STS_bit.CNT_OVF != 0){
            CT_IEP.TMR_GLB_STS_bit.CNT_OVF = 1;
            hdecay[i] ++;
            htime_ctr ++;
    }

    // Set the decay
    if(hdecay[i] > 1){
        ldecay[i] = UINT32_MAX;
    }
    else if(hdecay[i] == 1 && cur_time > ltime[i] ){
        ldecay[i] = UINT32_MAX;
    }
    else{
        ldecay[i] = cur_time - ltime[i];
    }


    // Do the debounce update
    if(!isHigh[i]){
        if((__R31 & mask) == mask){ // signal is high
            if(ltime_diff > TWO_THOUSAND_HZ){ // debounce
                isHigh[i] = 1;
                counts[i]++;

                hdelta[i] = hdecay[i];
                ldelta[i] = ldecay[i];


                htime[i] = htime_ctr;
                ltime[i] = CT_IEP.TMR_CNT;
            }
        }
    }
    else{

        if((__R31 & mask) != mask){ // signal is low
            if(ltime_diff > TWO_THOUSAND_HZ){ // debounce
                isHigh[i]=0;

                htime[i] = htime_ctr;
                ltime[i] = CT_IEP.TMR_CNT;;
            }
        }
    }
}



#endif /* OPTICAL_H_ */
