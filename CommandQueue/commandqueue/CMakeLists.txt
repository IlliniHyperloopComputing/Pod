
PYTHON_ADD_MODULE(command Command.cpp Command.h Event.h Event.cpp Defines.hpp SafeQueue.hpp Utils.cpp Utils.h)
FILE(COPY command.py DESTINATION .)
FILE(COPY set_GPIO.py DESTINATION .)
FILE(COPY Utils.py DESTINATION .)
ADD_TEST(NAME commandqueue COMMAND ${PYTHON_EXECUTABLE} command.py)
ADD_TEST(NAME set_gpio COMMAND ${PYTHON_EXECUTABLE} set_GPIO.py)