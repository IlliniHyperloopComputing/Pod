import json
import sys
import pprint
from enum import Enum

class Type(Enum):
  ACCELEROMETERX = 0
  TAPE_COUNT = 1
  OPTICAL = 2
  BRAKE_PRESSURE = 3
  THERMOCOUPLE = 4
  ACCELEROMETERYZ = 5
  RIDE_HEIGHT = 6
  BATTERY = 7
  CURRENT = 8
  PULL_TAB = 9
  TRUE_POSITION = 10
  TRUE_VELOCITY = 11
  TRUE_ACCELERATION = 12
  XMEGA_STATE = 13
  XMEGA_STATUS = 14
  XMEGA_RESPONDING = 15
  POD_STATE = 16
  SENSOR_STATUS = 17
  ACCEL_TIME = 18
  COAST_TIME = 19

lengths = {
  Type.ACCELEROMETERX : 6,
  Type.TAPE_COUNT : 2,
  Type.OPTICAL : 12,
  Type.BRAKE_PRESSURE : 2,
  Type.THERMOCOUPLE : 10,
  Type.ACCELEROMETERYZ : 4,
  Type.RIDE_HEIGHT : 6,
  Type.BATTERY : 4,
  Type.CURRENT : 4,
  Type.PULL_TAB : 1,
  Type.TRUE_POSITION : 8,
  Type.TRUE_VELOCITY : 8,
  Type.TRUE_ACCELERATION : 8,
  Type.XMEGA_STATE : 2,
  Type.XMEGA_STATUS : 2,
  Type.XMEGA_RESPONDING : 2,
  Type.POD_STATE : 1,
  Type.SENSOR_STATUS : 2,
  Type.COAST_TIME : 4,
  Type.ACCEL_TIME : 4
}
names = {
  Type.ACCELEROMETERX : "x_accelerometer",
  Type.TAPE_COUNT : "tape_count",
  Type.OPTICAL : "optical",
  Type.BRAKE_PRESSURE : "brake_pressure",
  Type.THERMOCOUPLE : "thermocouple",
  Type.ACCELEROMETERYZ : "yz_accelerometer",
  Type.RIDE_HEIGHT : "ride_height",
  Type.BATTERY : "battery",
  Type.CURRENT : "current",
  Type.PULL_TAB : "pull_tab",
  Type.TRUE_POSITION : "true_position",
  Type.TRUE_VELOCITY : "true_velocity",
  Type.TRUE_ACCELERATION : "true_accleration",
  Type.XMEGA_STATE : "xmega_state",
  Type.XMEGA_STATUS : "xmega_status",
  Type.XMEGA_RESPONDING : "xmega_responding",
  Type.POD_STATE : "pod_state",
  Type.SENSOR_STATUS : "sensor_status",
  Type.COAST_TIME : "coast_time",
  Type.ACCEL_TIME : "accel_time"

}
def get_type(val):
    try:
        int(val)
        return "int"
    except:
        pass
    try:
        float(val)
        return "float"
    except:
        pass
    try:
        if val == "true" or val == "false" or val == "t" or val == "f":
            return "boolean"
    except:
        pass
    return "string"

valid_props = {"host":"string",
               "port":"int",
                "print":"boolean",
                "outfile":"string"
                }

def get_property(prop):
    try:
        if prop in valid_props.keys():
            val = get_properties()[prop]
            type = valid_props[prop]
            if type == "boolean":
                if val == "true" or val == "t":
                    val = True
                else:
                    val = False
            elif type == "int":
                val = int(val)
            elif type == "float":
                val = float(val)
            return val
        elif prop == "help":
            return get_properties()["help"]
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return None


def get_command(command):
    try:
        command = get_properties()["commands"][command]

        val = command

        type = get_type(command)
        if type == "boolean":
            if val == "true" or val == "t":
                val = True
            else:
                val = False
        elif type == "int":
            val = int(val)
        elif type == "float":
            val = float(val)

        return val
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return None

def props_list(what):
    try:
        prop = get_properties()
        if what == "commands":
            print(prop["commands"])
            return True
        elif what == "receives":
            print(prop["receives"])
            return True
        return False
    except:
        return False
def get_receive(id):
    try:
        prop = get_properties()["receives"][id]
        return prop
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return None

def set_receive(id, values):
    try:
        props = get_properties()
        props["receive_len"][id] = {}
        for i in range(0,len(values)):
            if values[i] == "-l":
                props["receive_len"][id]["len"] = values[i+1]
            elif values[i] == "-n":
                props["receive_len"][id]["name"] = values[i+1]

        set_properties(props)
        return True
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return False


def set_command(command, values):
    try:
        final_value = ""
        for value in values:
            final_value = final_value + value + " "

        properties = get_properties()

        properties["commands"][command] = final_value

        set_properties(properties)
        return True
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return False


def set_property(prop, val):
    properties = get_properties()

    try:
        if prop in valid_props.keys() and valid_props[prop] is get_type(val):
                if get_type(val) == bool:
                    if val == "true" or val == "t":
                        val = True
                    else:
                        val = False
                properties[prop] = val
        else:
            return False
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return False

    set_properties(properties)
    return True


def get_properties():
    try:
        with open('props.json') as data_file:
            props = json.load(data_file)
        return props
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return None

def set_properties(new_props):
    try:
        with open('props.json', 'w') as outfile:
            json.dump(new_props, outfile)
        return True
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return False
