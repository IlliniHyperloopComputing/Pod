#ifdef XMEGA_H
#define XMEGA_h

enum Xmega_Command {
  // TODO list all possible commands
}

class Xmega {
  public:
    /**
    * Sets up connection to XMEGA
    * Creates internal data buffers
    **/
    Xmega();

    /**
    * Reads data from Spi
    * @return a buffer representing all the most recent sensor data
    **/
    uint8_t * read();

    /**
    * Writes a command to the Xmegas
    * Note: The command is not sent immediately
    **/
    void write(Xmega_Command);


  private:
    Spi * spi;
    /**
    * updates the internal buffers and sends the most recent command
    */
    void update();
}

#endif

