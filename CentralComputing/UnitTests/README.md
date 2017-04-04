# How to create a Unit Test! 

1.  Create a file called `MyExample_Test.h` and `MyExample_Test.cpp`
2.  Copy the contents of `Spi_Test.h` into your `.h` file. This is an easy starting point.
3.  Within your `.h` file: 
     1.  Change the include guards to match the name of your file
     2.  Change the name of the class to match the name of your file
     3.  Change the name of the constructor to match the name of your file
4.  Open your `.cpp` file. Use `Spi_Test.cpp` as a guide if needed
     1.  Define the `test(int argc, char ** argc)` as if it was the `main()` function
     2.  Define your constructor. The ONLY thing it should do is initialize the variables `name` and `cmd`. Both of these are printed out automatically in the "help" section if the unit tester is run without a command. `cmd` is used as the keyword to start your unit test.
