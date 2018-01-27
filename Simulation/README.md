# Simulator
Run with

>python3 main.py

Inorder to execute, two things should be provided
  - A path to the Pod's executable
  - A script file to be executed

For testing purposes, test.c can be compiled with __gcc__ and used as a stand in executable.
It will print out the data recieved from the pod into a file named out.t

## Scripting language
Using the scripting language for this simulator is not neccesary. You can run the simulator without it.
However, using this tool can allow you to set up quite interesting situations to test the pod under.

#### Variables
|Name| Can set| Can read| Can delta| Description|
|----|--------|---------|----------|------------|
|t   |no      |yes      |no      |the current epoch of simulation|
|dt  |no      |yes      |no      |the last time delta between two ticks of simulation|
|a   |yes     |yes      |no      |A component acceleration exceperienced by the pod (Not net accel)|
|na  |no      |yes      |no      |The net or total acceleration exceperienced by the pod. This variable is sent to the pod|
|v   |yes     |yes      |yes     |the velocity of the pod|
|x   |yes     |yes      |yes     |the distance from the start of the track|
|drag|yes     |yes      |yes     |the coeffecient of drag, drag is proportional to v^2|
|fric|yes     |yes      |yes     |the coeffecient of friction, friction is constant|
|motor_en|yes     |yes      |no      |if 0, motor is disabled. if > 0, motor is enabled|
|motor_thr|yes     |yes      |no      |the throttle of the motor 0 -> 255|
|brake_en|yes     |yes      |no      |if 0, brakes are disabled, else enable the brakes|
|pstate|no     |yes      |no      |the integer representation of the pod's state|
|track_len|no     |yes      |no      |the length of the track|

#### Syntax
The included script.sym file contains an example of syntax. Here I will document what this syntax is capable of.  
Please note, a block is defined as the list of commands placed between condtional checks. A simple block would be:
> @ t==3  
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;?Time is 3
##### Comments
Anything right of a # symbol is ignored and considered a comment. Only single line comments are supported in this manner.
##### Conditional Checks
>@ <conition 1>

This is how a single condition can be tested for.
>@ <conition 1> & <condition 2>

The syntax is also capable of boolean ___AND___ logic
>@ <conition 1> | <condition 2>

Aswell as boolean ___OR___ logic
>@ <conition 1> | <condition 2> & <condition 3>

Paranthesis are not supported, therefore the syntax always reads left to write, therefore this can be translated to </br>(condition 1 or condition 2) and condition 3
##### condition
Conditions are used inside conditional checks. There are currently 5 differernt symbols used by conditions:<\br>
* == This is used to denote left is equal to right
* \>= This is used to denote left greater than or equals right
* <= This is used to denote left lesser than or equals right
* \>  This is used to denote left greater than right
* \<  This is used to denote left lesser than right  
  
Currently, the left term should always be a readable variable. The right term can be a readable variable as well, however it can also be an integer/decimal number. By default, these numbers are held at 3 places of percision (e.g 4 -> 4.000). However, if decimal places are provided, then the percision is overwritten (e.g. 4.0 -> 4.0). This is required because of how the simulator calculates these values

It's also important to note that the left and right terms ___can not___ have spaces between them and the condition symbol.  
These are both fine:
> @ t>=5  
> @t>=5

##### Set value
The only variables that can be changed in value are ones that are writable. There are four operators that can be used to change the value of a variable:
* =
* -=
* +=
* \*=  
  
These operators should be self explanatory. Please note, just as with conditions, do not places spaces between each left/right term(s) and the symbol. The left term should always be a writable variable, the right term can be either a value, or a readable variable.
##### Delta operator
the % symbol is used to denote a constant change/second. Anything that can delta, can be changed by some value over time.
For example, %x is equivalent to v. Please note:
* %x is equivalent to v
* %v is equivalent to a
##### Print operator
The ? symbol is used to denote print something. There are a few cases to consider:
* ? alone denotes print nothing. This can be useful in saving a snapshot of the simulation
* ?<TEXT> will print the TEXT message to command line.
* ?:<Varname> will print out the value of the readable variable defined after the colon
* ?\* will print out a debug status of all the internal states used by the simulation

##### Delete Block operator
Sometimes, you only want a block to execute once. Often the same == condition will return true within the space of a few milliseconds. Which could result in the block executing over a hundred times. To reduce this risk, placing a ^ symbol inside a block will mark it for deletion, resulting in that block __never__ executing again.
##### Failure operator
When a failure should occur, for example hitting the end of the track, the ! symbol is used to throw an exception which ends the simulation. The ! can be followed by a message, giving context to the failure.  
Example:
> @ x>track_len  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;!Pod hit the end of the track</br>

##### Success operator
The successful completion of a simulation can be denoted using __(:__ This allows for success checks. Without a block containing this, a simulation may never end
