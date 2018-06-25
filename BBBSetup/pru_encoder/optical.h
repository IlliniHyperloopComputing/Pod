/*
 * optical.h
 *
 *  Created on: Jun 24, 2018
 *      Author: rgw3d
 */

#ifndef OPTICAL_H_
#define OPTICAL_H_

#define debounce(mask, count, high, htime, ltime) { \
    if(!high){\
        if((__R31 & mask) == mask){\
            uint32_t htime_diff = htime_ctr - htime + CT_IEP.TMR_GLB_STS_bit.CNT_OVF;\
            uint32_t ltime_diff = CT_IEP.TMR_CNT - ltime;\
            if(htime_diff > 0 || ltime_diff > 100000){\
                high = 1;\
                count++;\
            }\
        }\
    }\
    else{\
        if((__R31 & mask) != mask){\
            uint32_t htime_diff = htime_ctr - htime + CT_IEP.TMR_GLB_STS_bit.CNT_OVF;\
            uint32_t ltime_diff = CT_IEP.TMR_CNT - ltime;\
            if(htime_diff > 0 || ltime_diff > 100000){\
                high=0;\
            }\
        }\
    }\
}

#endif /* OPTICAL_H_ */
