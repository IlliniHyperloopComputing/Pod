
PYTHON_ADD_MODULE(command Command.cpp Command.h Event.h Event.cpp Defines.hpp SafeQueue.hpp Utils.cpp Utils.h)
FILE(COPY command.py DESTINATION .)
ADD_TEST(NAME commandqueue COMMAND ${PYTHON_EXECUTABLE} command.py)
