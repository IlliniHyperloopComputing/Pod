#include "Spi.h"
#include "Spi_Test.h"
#include "Utils.h"
#include <time.h>

int Spi_Test::test(int argc, char** argv){

  print_test("%s\t\t%s\t%c\n",argv[0], argv[1], argv[1][0]);
  if(argv[1][0] == '1'){
    print_test("I am in the correct function");
    uint8_t bpi1[] = {2,2,2,2,2,2,2,2,2,2};
    Xmega_Setup x1 = {"/dev/spidev1.0", 10, bpi1, 500000, 8};
    uint8_t bpi2[] = {2,2,2};
    Xmega_Setup x2 = {"/dev/spidev1.1", 3, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {1,X_C_NONE, X_R_SENSOR};

    double elapsed = 0;
    int err = 0;
    struct timespec ts, ts2;
    int iters = 1000;
    for(int i = 0; i< iters; i++){
      clock_gettime(CLOCK_MONOTONIC,&ts);
      int result = spi.transfer(xt);
      clock_gettime(CLOCK_MONOTONIC, &ts2);
      if( result != 0) err++;
      usleep(10);
      elapsed += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
    }
    print_test("time elapsed: %lf\t Average time: %lf\n",elapsed, elapsed/iters);
    print_test("err: %d\n", err);
    for(int i = 0; i < 3; i++){
      print_test("got data: idx: %x\t data:%x\tdata:%x\n",i, spi.get_data(0,i), spi.get_data(1,i));
    }

  }
  else{
    uint8_t bpi1[] = {2,2,2,2,2,2,2,2,2,2};
    Xmega_Setup x1 = {"/dev/spidev1.0", 10, bpi1, 500000, 8};
    uint8_t bpi2[] = {2,2,2,2,2,2,2,2,2,2};
    Xmega_Setup x2 = {"/dev/spidev1.1", 10, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {0,X_C_NONE, X_R_SENSOR};

    int x1se =0;
    int x2se =0;
    int x1re =0;
    int x2re =0;
    int nume = 0;
    int bytes_sent = 0;
    int bytes_recv = 0;

    double elapsed = 0;
    struct timespec ts, ts2;
    int iters = 1000;
    for(int i = 0; i< iters; i++){
      clock_gettime(CLOCK_MONOTONIC,&ts);
      int result = spi.transfer(xt);
      clock_gettime(CLOCK_MONOTONIC, &ts2);
      elapsed += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
      if(result & 0b01) x1se++;
      else { bytes_sent+= 2;}
      if(result & 0b10) x2se++;
      else { bytes_sent+= 2;}
      if(result & 0b100) x1re++;
      else if(!(result & 0b01)){ bytes_recv+=8;}
      if(result & 0b1000) x2re++;
      else if(!(result & 0b10)){ bytes_recv+=20;}
      if(result) nume++;
    }
    print_test("time elapsed: %lf\t Average time: %lf\n",elapsed, elapsed/iters);
    print_test("num errors:%d\t x1se:%d\t x2se:%d\t x1re:%d\t x2re:%d\t\n", nume, x1se, x2se, x1re, x2re);
    print_test("bytes_sent:%d \t bytes_recv:%d\n", bytes_sent, bytes_recv);
    print_test("(per second) bytest sent throughput: %lf\t bytes recv throughput%lf\n", bytes_sent/elapsed, bytes_recv/elapsed);
  }

  //for(int i = 0; i < 10; i++){
  //  print_test("got data: idx: %x\t data:%x\tdata:%x\n",i, spi.get_data(0,i), spi.get_data(1,i));
  //}

  return 0;
}

Spi_Test::Spi_Test(){
  name = "Spi Test";
  cmd  = "spi";
}
