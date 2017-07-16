#include "Spi.h"
#include "Spi_Test.h"
#include "Utils.h"
#include <time.h>

int Spi_Test::test(int argc, char** argv){

  if(argv[1][0] == '1'){
    uint8_t bpi1[] = {2,2,2,2,4};
    Xmega_Setup x1 = {"/dev/spidev1.0", 5, bpi1, 8000000, 8};
    uint8_t bpi2[] = {2,2,2};
    Xmega_Setup x2 = {"/dev/spidev1.1", 3, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {0,X_C_NONE, X_R_ALL};

    double elapsed = 0;
    int err1 = 0;
    int err2 = 0;
    struct timespec ts, ts2;
    int iters = 1000;
    for(int i = 0; i< iters; i++){
      clock_gettime(CLOCK_MONOTONIC,&ts);
      int result = spi.transfer(xt);
      clock_gettime(CLOCK_MONOTONIC, &ts2);
      if(result & 0b01) err1++;
      if(result & 0b10) err2++;
      usleep(5000);//5 milliseconds
      elapsed += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
    }
    print_test("time elapsed: %lf\t Average time: %lf\n",elapsed, elapsed/iters);
    print_test("err1: %d\t err2:%d\n", err1,err2);
    for(int i = 0; i < 3; i++){
      print_test("got data: idx: %x\t data:%x\tdata:%x\n",i, spi.get_data(0,i), spi.get_data(1,i));
    }

    int16_t adc1 = spi.get_data(1,0);
    print_test("Got idx0: %d\n", adc1);

  }
  else{
    uint8_t bpi1[] = {2,2,2,2,4};
    Xmega_Setup x1 = {"/dev/spidev1.0", 5, bpi1, 8000000, 8};
    uint8_t bpi2[] = {2,2,2,2,4};
    Xmega_Setup x2 = {"/dev/spidev1.1", 5, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {0,X_C_NONE, X_R_ALL};
      
    //print_test("Asking for state\n");
    //int result = spi.transfer(xt);
    //print_test("Result: %d\n", result);
    //assert(result == 0);

    //print_test("\nAsking for sensor status\n");
    //xt.req = X_R_SENSOR_STATUS;
    //result = spi.transfer(xt);
    //print_test("Result: %d\n", result);
    //assert(result == 0);

    //print_test("\nAsking for NONE\n");
    //xt.req = X_R_NONE;
    //result = spi.transfer(xt);
    //print_test("Result: %d\n", result);
    //assert(result == 0);

    print_test("\nAsking for ALL\n");
    xt.req = X_R_ALL;
    int result = spi.transfer(xt);
    print_test("Result: %d\n", result);
    assert(result == 0);

    print_test("\nSensor_status: %d\n", spi.get_sensor_status(1));
    print_test("State: %d\n", spi.get_state(1));
    for(int i = 0; i < 5; i++){
      print_test("Data: idx: %x\t xmega1:%d\txmega2:%d\n",i, spi.get_data(0,i), spi.get_data(1,i));
    }

  }
}

Spi_Test::Spi_Test(){
  name = "Spi Test";
  cmd  = "spi";
}
