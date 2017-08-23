import socket
import array
import sys
from propertyHandler import set_property, get_property, get_command, set_command, get_receive, set_receive, props_list, Type, lengths, names
from struct import unpack
from _thread import *

mySocket = None

def get_socket():
    return mySocket
def set_socket(mine):
    global mySocket
    mySocket = mine

listen_thread = None

def get_listen_thread():
    return listen_thread
def set_listen_thread(mine):
    global listen_thread
    listen_thread = mine

def threaded_listen(conn):
    while True:
        d = conn.recv(1)
        if not d:
            break
        id = Type(unpack('B', d)[0])
        length = lengths[id]
        data = conn.recv(length)
        string = parse_data(id, length, data)	
    conn.close()
    set_socket(None)
    set_listen_thread(None)

def parse_data(id, length, data):
	print(names[id])
	if id == Type.OPTICAL:
		# [double][uint32]
		double = unpack('d', data[0:8])[0]
		count = unpack('I', data[8:12])[0]
		print("RPM : " + str(double))
		print("Count : " + str(count))
		pass
	elif id == Type.TRUE_POSITION or id == Type.TRUE_VELOCITY or id == Type.TRUE_ACCELERATION:
		# [double]
		value = unpack('d', data)[0]
		print("Value : " + str(value))
	elif id == Type.PULL_TAB or id == Type.XMEGA_STATE or id == Type.XMEGA_STATUS or id == Type.XMEGA_RESPONDING or id == Type.POD_STATE:
		# multiple [uint8]
		count = length
		for i in range(0, count):
			value = unpack('B', data[i:i+1])[0]
			print(str(i) + " : " + str(value))
	else:
		# multiple [uint16]
		count = length//2
		for i in range(0, count):
			value = unpack('H', data[i*2:i*2+2])[0]
			print(str(i) + " : " + str(value))
	
	print()

def start_listening():
    try:
        if get_listen_thread() is None:
            listen_thread = start_new_thread(threaded_listen, (get_socket(),))
            set_listen_thread(listen_thread)
            return listen_thread
        else:
            return None
    except:
        e = sys.exc_info()[0]
        print("error:", e)
        return None

def create_connection(host, port):
    try:
        mySocket = socket.socket()
        mySocket.connect((host, port))
        set_socket(mySocket)
        return True
    except:
        e = sys.exc_info()[0]
        print("error:",e)
        return False

def send_command(command):
    message = get_command(command)
    message = message.to_bytes(1, 'big')
    get_socket().send(message)


def parse_instruction(line):
    instructions = line.split()
    for i in range(0, len(instructions)):
        instruct = instructions[i].lower()
        if instruct == "list":
            what = instructions[i+1]
            return props_list(what)

        if instruct == "set":
            prop = instructions[i + 1]
            val = instructions[i + 2]
            if prop == "command":
                name = val
                vals = instructions[i+3:]
                worked = set_command(name, vals)
            elif prop == "receive":
                id = val
                vals = instructions[i+3:]
                worked = set_receive(id, vals)
            else:
                worked = set_property(prop, val)
            return worked
        elif instruct == "get":
            prop = instructions[i + 1]
            if prop == "command":
                command = instructions[i + 2]
                ret = get_command(command)
                if ret is not None:
                    print(command + ":" + str(ret))
                    return True
                else:
                    return False
            if prop == "receive":
                command = instructions[i + 2]
                ret = get_receive(command)
                if ret is not None:
                    print("receive for id " + command + " is " + str(ret))
                    return True
                else:
                    return False
            else:
                val = get_property(prop)
                if val is not None:
                    print(prop + ':' + str(val))
                    return True
                else:
                    print(prop + ' is undefined')
                    return False
        elif instruct == "connect":
            host = get_property("host")
            port = get_property("port")
            worked = create_connection(host, port)
            if worked:
                print("connected to " + host + ":" + str(port))
            return worked
        elif instruct == "listen":
            start_listening()
            return get_listen_thread() is not None
        elif instruct == "send":
            if get_socket() is None:
                return False
            else:
                command = instructions[i + 1]
                send_command(command)
                return True



def Main():
    while True:
        line = input(" -> ")
        try:
            worked = parse_instruction(line)
        except:
            e = sys.exc_info()[0]
            print("error:", e)
            worked = False
        if not worked:
            print("error occured")
            # create_connection(host, port)


if __name__ == '__main__':
    Main()
