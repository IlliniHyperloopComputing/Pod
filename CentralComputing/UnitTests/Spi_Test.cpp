#include "Spi.h"
#include "Spi_Test.h"
#include "Utils.h"
#include <time.h>
#include <iostream>

int Spi_Test::test(int argc, char** argv){
  
  if(!argv[1]) return 1;

  char l = argv[1][0];

  if(l == '1'){
    uint8_t bpi1[] = {2,2,2,2,4,4};
    Xmega_Setup x1 = {"/dev/spidev1.0", 6, bpi1, 500000, 8};
    uint8_t bpi2[] = {2,2,2,2,2,2,2,2,2,2,2,2,1};
    Xmega_Setup x2 = {"/dev/spidev1.1", 13, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {0,X_C_NONE, X_R_ALL};

  
    double overelapsed = 0;
    double overelapsed2 = 0;
    int overerr1 = 0;
    int overerr2 = 0;
    int overerr3 = 0;
    int overerr4 = 0;
    int overiters = 10;

    int iters = 1000;
    for(int z = 0; z<overiters; z++){

      double max_n = 0;
      double min_n = 10000000;
      double elapsed = 0;
      int err1 = 0;
      int err2 = 0;

      double max_n2 = 0;
      double min_n2 = 10000000;
      double elapsed2 = 0;
      int err3 = 0;
      int err4 = 0;

      struct timespec ts, ts2;
      for(int i = 0; i< iters; i++){
        xt.device = 0;
        clock_gettime(CLOCK_MONOTONIC,&ts);
        int result = spi.transfer(xt);
        clock_gettime(CLOCK_MONOTONIC, &ts2);
        if(result & 0b01) err1++;
        if(result & 0b10) err2++;
        elapsed += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
        double nsec_total = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
        if(nsec_total < min_n) 
          min_n = nsec_total;
        if(nsec_total > max_n) 
          max_n = nsec_total;

        xt.device = 1;
        clock_gettime(CLOCK_MONOTONIC,&ts);
        result = spi.transfer(xt);
        clock_gettime(CLOCK_MONOTONIC, &ts2);
        if(result & 0b01) err3++;
        if(result & 0b10) err4++;

        usleep(5000);//5 milliseconds
        elapsed2 += (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
        nsec_total = (ts2.tv_sec - ts.tv_sec) + (ts2.tv_nsec-ts.tv_nsec)/1000000000.0;
        if(nsec_total < min_n2) 
          min_n2 = nsec_total;
        if(nsec_total > max_n2) 
          max_n2 = nsec_total;
        
      }
      print_test("=====================================\n");

      print_test("time elapsed: %lf\t Average time: %lf\n",elapsed, elapsed/iters);
      print_test("err1: %d\t err2:%d\n", err1,err2);
      print_test("Fail rate: %f%%\n", (err1+err2)/((float)iters)*100);
      print_test("Max Time: %f     Min Time: %f\n", max_n*1000, min_n*1000);

      print_test("\n");

      print_test("time elapsed: %lf\t Average time: %lf\n",elapsed2, elapsed2/iters);
      print_test("err1: %d\t err2:%d\n", err3,err4);
      print_test("Fail rate: %f%%\n", (err3+err4)/((float)iters)*100);
      print_test("Max Time: %f     Min Time: %f\n", max_n2*1000, min_n2*1000);

      print_test("=====================================\n");

      overelapsed +=elapsed;
      overerr1 += err1;
      overerr2 += err2;

      overelapsed2 +=elapsed2;
      overerr3 += err3;
      overerr4 += err4;
    }

    print_test("==FINAL==\nElapsed: %f   avg elapsed: %f    avg trans: %f\nerr1: %d\t err2:%d\nerr %: %f%%\n",overelapsed,overelapsed/((double)overiters), overelapsed/((double)overiters)/iters, overerr1, overerr2, (overerr1+overerr2)/((double)iters*overiters)*100);
    print_test("Elapsed: %f   avg elapsed: %f    avg trans: %f\nerr1: %d\t err2:%d\nerr %: %f%%\n",overelapsed2,overelapsed2/((double)overiters), overelapsed2/((double)overiters)/iters, overerr3, overerr4, (overerr3+overerr4)/((double)iters*overiters)*100);
    return 1;


  }
  else if(l == '2'){
    uint8_t bpi1[] = {2,2,2,2,4,4};
    Xmega_Setup x1 = {"/dev/spidev1.0", 6, bpi1, 500000, 8};
    uint8_t bpi2[] = {2,2,2,2,2,2,2,2,2,2,2,2,1};
    Xmega_Setup x2 = {"/dev/spidev1.1", 13, bpi2, 500000, 8};
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

    print_test("\nAsking for ALL\n");
    xt.device=1;
    result = spi.transfer(xt);
    print_test("Result: %d\n", result);
    assert(result == 0);

    print_test("\nSensor_status: %d\n", spi.get_sensor_status(1));
    print_test("State: %d\n", spi.get_state(1));
    for(int i = 0; i < 5; i++){
      print_test("Data: idx: %x\t xmega1:%d\txmega2:%d\n",i, spi.get_data(0,i), spi.get_data(1,i));
    }

  } else if(l == '3'){
    uint8_t bpi1[] = {2,2,2,2,4,4};
    Xmega_Setup x1 = {"/dev/spidev1.0", 6, bpi1, 500000, 8};
    uint8_t bpi2[] = {2,2,2,2,2,2,2,2,2,2,2,2,1};
    Xmega_Setup x2 = {"/dev/spidev1.1", 13, bpi2, 500000, 8};
    Spi spi(&x1, &x2);
    Xmega_Transfer xt = {0,X_C_NONE, X_R_ALL};

    while(1){
      std::string bleh;
      cin >> bleh;

      print_test("\nAsking for ALL\n");
      xt.req = X_R_ALL;
      xt.device = 0;
      int result = spi.transfer(xt);
      print_test("Result: %d\n", result);
      //assert(result == 0);

      print_test("\nAsking for ALL\n");
      xt.req = X_R_ALL;
      xt.device=1;
      result = spi.transfer(xt);
      print_test("Result: %d\n", result);
      //assert(result == 0);
      print_test("\n=============\n");


      print_test("Xmega1\n");
      print_test("X0: %f\n", spi.get_data(0,0)/32768.0*4.096);
      print_test("X1: %f\n", spi.get_data(0,1)/32768.0*4.096);
      print_test("X2: %f\n", spi.get_data(0,2)/32768.0*4.096);
      print_test("PRESSURE: %f\n", spi.get_data(0,3)/32768.0*4.096);
      print_test("OPTICAL RPM: %f\n", 60.0 / (spi.get_data(0,4) * 0.00003051757));
      print_test("OPTICAL Rotations: %lu\n", spi.get_data(0,5));

      print_test("\n");
      print_test("Xmega2\n");
      print_test("Y: %f\n", spi.get_data(1,0)/32768.0*4.096);
      print_test("Z: %f\n", spi.get_data(1,1)/32768.0*4.096);
      print_test("RH0: %f\n", spi.get_data(1,2)/32768.0*4.096);
      print_test("RH1: %f\n", spi.get_data(1,3)/32768.0*4.096);
      print_test("RH2: %f\n", spi.get_data(1,4)/32768.0*4.096);
      print_test("BAT0: %f\n", spi.get_data(1,5)/32768.0*4.096);
      print_test("BAT1: %f\n", spi.get_data(1,6)/32768.0*4.096);
      print_test("THERMO0: %f\n", spi.get_data(1,7)*0.25);
      print_test("THERMO1: %f\n", spi.get_data(1,8)*0.25);
      print_test("THERMO2: %f\n", spi.get_data(1,9)*0.25);
      print_test("THERMO3 external: %f\n", spi.get_data(1,10)*0.25);
      print_test("THERMO3 internal: %f\n", spi.get_data(1,11)*0.0625);
      print_test("RETRO Count: %d\n", spi.get_data(1,12)) ;
    }

    return 0;
  }
  return 1;
}

Spi_Test::Spi_Test(){
  name = "Spi Test";
  cmd  = "spi";
}
