# Unit Testing

### Creating a Unit Test -- the Gist:
* Create a class within appropriatly named `****_Test.h` and `****_Test.cpp` files within the `UnitTests/` directory
* The class must publicaly extend the parent class `Unit_Test`. That is: `class ****_Test: public Unit_Test`
* The `Unit_Test` class provides one method to override: `test(int argc, char ** argv)`. This method acts as your `main()`, and you can do whatever you want inside this method. 
* In order to call your test from the testing suite and for easy identification, you must define `name` and `cmd` in the default constructor for you class. These should be the only things you define in your constructor. Both of these are printed out automatically in the "help" section if the unit tester is run without a command. `cmd` is used as the keyword to start your unit test.
* Once you have setup everything (defined test(), defined the default constructor initalizing `name` and `cmd` strings), you must include your new test in `Unit_Test.cpp`. Include your `.h`, and add your object to the vector.

### Creating a Unit Test -- Example: 
1.  Create a file called `MyExample_Test.h` and `MyExample_Test.cpp`
2.  Copy the contents of `Spi_Test.h` into your `.h` file. This is an easy starting point.
3.  Within your `.h` file: 
     1.  Change the include guards to match the name of your file
     2.  Change the name of the class to match the name of your file
     3.  Change the name of the constructor to match the name of your file
4.  Open your `.cpp` file. Use `Spi_Test.cpp` as a guide if needed
     1.  Define the `test(int argc, char ** argv)` as if it was the `main()` function
     2.  Define your constructor. The ONLY thing it should do is initialize the variables `name` and `cmd`. 
5.  Open `Unit_Test.cpp`
     1.  Add your `.h` to the top of this file
     2.  Add your object to the vector at the top of the `test()` function. Follow the examples there.
     
### Running your test:
1.  From the CentralComputing directory, run `make clean` then `make build_test`
2.  Run `./build_test` And see if your new test shows up in the "help" section
3.  Run `./build_test cmd`, replacing cmd with whatever you defined cmd to be, and your test will start!
