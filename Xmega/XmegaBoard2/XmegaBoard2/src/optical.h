/*
 * optical.h
 *
 * Created: 3/4/2018 12:57:12 PM
 *  Author: rgw3d
 */ 

#ifndef OPTICAL_H_
#define OPTICAL_H_

//#include <asf.h>

#define read_val(port, mask, pos) (port & mask) >> pos;	

//This is a macro for the common case of handling "optical"
//sensor data. That is, input data from a pin that is either
//on or off. 
//This macro function is really just a template, so that the
//main body of code is easier to read.
#define COOLDOWN 2
#define debounce(val, high, cooldown, cur_time, old_time, delta) { \
	if(!high){\
		if(val){\
			cooldown++;\
			if(cooldown>COOLDOWN){\
				cur_time = rtc_get_time();\
				delta = (uint16_t) (cur_time - old_time);\
				old_time = cur_time;\
				high=1;\
				cooldown=0;\
			}\
		}\
		else{\
			cooldown=0;\
		}\
	}\
	else{\
		if(!val){\
			cooldown++;\
			if(cooldown>COOLDOWN){\
				high=0;\
				cooldown=0;\
			}\
		}\
		else{\
			cooldown=0;\
		}\
	}\
}

#define debounce_count(val, count, high, cooldown, cur_time, old_time, delta) { \
	if(!high){\
		if(val){\
			cooldown++;\
			if(cooldown>COOLDOWN){\
				cur_time = rtc_get_time();\
				delta = cur_time - old_time;\
				old_time = cur_time;\
				count++;\
				high=1;\
				cooldown=0;\
			}\
		}\
		else{\
			cooldown=0;\
		}\
	}\
	else{\
		if(!val){\
			cooldown++;\
			if(cooldown>COOLDOWN){\
				high=0;\
				cooldown=0;\
			}\
		}\
		else{\
			cooldown=0;\
		}\
	}\
}




#endif /* OPTICAL_H_ */