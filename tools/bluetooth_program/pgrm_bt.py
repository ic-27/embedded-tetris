import argparse
import serial
import time

#arg parsing
parser = argparse.ArgumentParser(description='Program HC-05 chips or simulate tetris gameplay')
parser.add_argument('port', type=str, nargs='?',
                    default='/dev/ttyUSB0', help='Port usb to serial device is attached to')
parser.add_argument('-s', '--sim', nargs='?', default=0,
                    help='Simulate 2P tetris instead of programming HC-05 chips')
args = parser.parse_args()
port = args.port
sim = args.sim

baud_rate = 38400
if(sim):
    baud_rate = 9600
ser = serial.Serial(port, baud_rate, timeout=1)  # 38400 when programming, 9600 during normal operation

at_commands = ["AT+ORGL", "AT+ROLE=", "AT+INQ", "AT+BIND=<SLAVE_ADDR>"]

def send_at_command(command):
    '''
    :param command: string containing the command to send
    :return: None
    '''
    ser.write((command+"\r\n").encode())

def send_and_parse_cmd(command):
    '''
    :param command: string containing the command to send
    :return bool: success or failure
    '''
    print("Sent command: {0}".format(command))
    send_at_command(command)
    at_response = get_at_response()
    if("OK" in at_response):
        return at_response #presence of string evaluates to true
    print("Abnormal response {0} sending command \"{1}\"".format(at_response, command))
    return False
    
def send_at_commands():
    '''
    :param master_slave: unsigned int in which 1 is master, 0 is slave
    :return: None
    '''

    #start programming master chip
    cmd_success = send_and_parse_cmd("AT+ORGL")
    if(not cmd_success):
        return

    cmd_success = send_and_parse_cmd("AT+ROLE=1")
    if(not cmd_success):
        return
    
    cmd_success = send_and_parse_cmd("AT+UART=9600,1,0")
    if(not cmd_success):
        returnxo

    cmd_success = send_and_parse_cmd("AT+ADDR?")
    if(not cmd_success):
        return
    at_addr_response = cmd_success
    at_addr_response = at_addr_response.split("\r\n")[0]
    master_addr_to_send = (at_addr_response.split(":", 1)[1]).replace(":", ",")
    print("The master HC-05 chip's address was found to be {0}".format(master_addr_to_send))
    print("The previous commands were sent successfully")

    #now we program the slave chip
    user_input = input("\nPlease plug in the HC-05 chip that will be designated as the slave.\nPress any key and press enter to continue programming.")

    cmd_success = send_and_parse_cmd("AT+ORGL")
    if(not cmd_success):
        return

    cmd_success = send_and_parse_cmd("AT+ROLE=0")
    if(not cmd_success):
        return
    
    cmd_success = send_and_parse_cmd("AT+UART=9600,1,0")
    if(not cmd_success):
        return

    cmd_success = send_and_parse_cmd("AT+ADDR?")
    if(not cmd_success):
        return
    at_addr_response = cmd_success
    at_addr_response = at_addr_response.split("\r\n")[0]
    slave_addr_to_send = (at_addr_response.split(":", 1)[1]).replace(":", ",")
    
    cmd_success = send_and_parse_cmd("AT+BIND="+master_addr_to_send)
    if(not cmd_success):
        return

    #finish programming master chip
    user_input = input("\nPlease plug in the master HC-05 chip to finish programming\nPress any key and press enter to finish programming.")

    cmd_success = send_and_parse_cmd("AT+BIND="+slave_addr_to_send)
    if(not cmd_success):
        return
    
def get_at_response():
    time.sleep(0.1) # wait for chip to properly receive uart msg first
    ret_val = ""
    while(ser.in_waiting):
        ret_val += ser.readline().decode()
    return ret_val
        
if(__name__ == "__main__"):
    print("This is a quick 'n dirty program to program the HC-05 bluetooth chip, for the Atmega 2P tetris game.")
    print("Please ensure that both bluetooth chips are on to ensure the correct functionality.")
    
    print("Press \"h\" for help, \"q\" to quit")
    while(1):
        user_input = input("\nEnter a command: ")
        if(user_input == "q"):
            break
        elif(user_input == "h"):
            print("The possible commands are:")
            print("\"m\": Program a HC-05 chip as master")
            print("\"s\": Program a HC-05 chip as slave")
            print("\"t\": Test HC-05 chip with AT+ROLE? command")
            print("\"n\": Test if HC-05 chip sees any neighbors also named HC-05")
        elif(user_input == "s"):
            print("Starting programming process. Insert the HC-05 chip that will be designated as master first.")
            send_at_commands()
        elif(user_input == "t"):
            print("Testing sending and receiving a response from HC-05")
            print("If working, you should see a response below this line.")
            #send_at_command("AT+ROLE?")
            send_at_command("AT+ADDR?")
            print(get_at_response())
        elif(user_input == "a"):
            #ser.write((command+"\r\n").encode())
            print("sending")
            ser.write("2\r\n".encode())
        else:
            print("Unknown command")
    ser.close()


