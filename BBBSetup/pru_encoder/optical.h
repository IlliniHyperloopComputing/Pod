/*
 * optical.h
 *
 *  Created on: Jun 24, 2018
 *      Author: rgw3d
 */

#ifndef OPTICAL_H_
#define OPTICAL_H_


extern uint32_t htime_ctr;

extern uint32_t counts[11]; //Number of times we have seen the signal
extern uint32_t htime[11]; //upper 32 bits of last time there was a high or low signal
extern uint32_t ltime[11]; //lower 32 bits of last time there was a high or low signal

extern uint32_t hlasthigh[11];
extern uint32_t llasthigh[11]; // These are what the time deltas are based off of.

extern uint32_t hdelta[11]; //time between the most recently seen signal and the signal before that one.
extern uint32_t ldelta[11];

extern uint8_t isHigh[11]; //Signal is currently high, need it to go low.

uint32_t htime_diff = 0;
uint32_t ltime_diff = 0;

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


inline
void debounce(const uint32_t mask, const uint32_t i) {
    if(!isHigh[i]){
        if((__R31 & mask) == mask){

            diff_time(htime_diff, ltime_diff, htime[i], ltime[i]);

            if(htime_diff > 0 || ltime_diff > 100000){
                isHigh[i] = 1;
                counts[i]++;

                htime[i] += htime_diff;
                ltime[i] += ltime_diff;

                diff_time(hdelta[i], ldelta[i], hlasthigh[i], llasthigh[i]);

                hlasthigh[i] = htime[i];
                llasthigh[i] = ltime[i];
            }
        }
    }
    else{
        if((__R31 & mask) != mask){
            diff_time(htime_diff, ltime_diff, htime[i], ltime[i]);

            if(htime_diff > 0 || ltime_diff > 100000){
                isHigh[i]=0;

                htime[i] += htime_diff;
                ltime[i] += ltime_diff;
            }
        }
    }
}



#endif /* OPTICAL_H_ */
