class Utils:
    @staticmethod
    def set_GPIO(GPIONumber, switchVal):
        start = "/sys/class/gpio/gpio"
        integer = str(GPIONumber)
        end = "/value"
        #path = start + integer + end
        path = "value"
        f = open(path, "w")
        if switchVal:
            f.write("1")
        else:
            f.write("0")
        f.close()
        return True
