import sys

import serial
import serial.tools.list_ports
import time
import asyncio


HARDWARE_ID = "2341:8037"
LINE_TERMINATOR = "\r\n"
BAUDRATE = 9600
TIMEOUT = 1 #s

connection = None

def connect_serial():
    ports = serial.tools.list_ports.comports()
    
    my_port = ""
    for port, desc, hwid in sorted(ports):
        if HARDWARE_ID in hwid:
            my_port = port
            break
    
    if my_port == "":
        raise Exception("Could not find valid port to connect to.")
    
    try:
        return serial.Serial(port=my_port, baudrate=BAUDRATE, timeout=TIMEOUT)
    except serial.serialutil.SerialException:
        #Port may be in use - close and try again
        raise Exception("Port in use. This may be due to an aborted connection. Try replugging the sensor.")

def send_command(cmd):
    out = str.encode(cmd + LINE_TERMINATOR)
    connection.write(out)

def read_response(n):
    # Get n lines of data from the serial connection and strip line terminator
    for i in range(n):
        raw = connection.readline().decode('utf-8')
        print(raw.replace(LINE_TERMINATOR, ""))

def set_pump_voltage(pump, voltage):
    #Set the voltage for the specified pump
    #Pumps 1-4, Voltages 0-250
    
    v_command = "P" + str(pump) + "V" + str(voltage)
    send_command(v_command)
    read_response(1)

async def create_droplet_time(pump, on_voltage, off_voltage, duration):
    set_pump_voltage(pump, on_voltage)
    time.sleep(duration)
    set_pump_voltage(pump, off_voltage)

def set_all_pumps_voltage(v_pump1, v_pump2, v_pump3, v_pump4):
    # Each pump 0-250, filled to 3 digits
    
    v_cmd = "PA" + str(v_pump1).zfill(3) + "#" + str(v_pump2).zfill(3)+ "#"  + str(v_pump3).zfill(3)+ "#"  + str(v_pump4).zfill(3)
    send_command(v_cmd)
    read_response(1)

def set_frequency(freq):
    #Set the frequency for all connected pumps
    #Sensible range: 0...700
    f_command = "F" + str(freq)
    send_command(f_command)
    read_response(1)
    
def initialise():
    #Set up the pump controller and default to inactive pumps
    
    #Find device and open serial connection
    connection = connect_serial()
    
    #Print device info
    send_command("V")
    read_response(1)
    
    #Set Highdriver-4
    send_command("SELECTQUADDRIVER")
    read_response(1)
    
    #Set driver frequency
    set_frequency(50)
    
    #Initialise all pumps to zero
    set_all_pumps_voltage(0, 0, 0, 0)
    
    
if __name__ == '__main__':
    
    connection = connect_serial()
    initialise()
    
    input("Initialisation complete. Press any key to start sample program...")
    
    #Set a background flow on Pump 1 and give it 5s to settle
    set_pump_voltage(1, 250)
    time.sleep(5)
    
    active_v = 80 #V
    idle_v = 10 #V
    iteration_delay = 3 #s
    
    #Increasing single-channel droplet size on each channel (2 through 4)
    for pump in range(2, 5):
        print("Producing 5 droplets with increasing size with pump " + str(pump))
        for wait in range(1, 6):
            set_pump_voltage(pump, active_v)
            time.sleep(wait/10)
            set_pump_voltage(pump, idle_v)
            time.sleep(iteration_delay)
            
    #Mixed droplets - fading from pump 2 to 3
    print("Producing 10 droplets, fading from mostly pump 2 to mostly pump 3.")
    for wait in range(1, 11):
        if asyncio.get_event_loop().is_running(): #This is need for Spyder
            asyncio.create_task(create_droplet_time(2, active_v, idle_v, (10-wait)/20))
        else:
            asyncio.run(create_droplet_time(2, active_v, idle_v, (10-wait)/20))
        
        if asyncio.get_event_loop().is_running(): #This is need for Spyder
            asyncio.create_task(create_droplet_time(3, active_v, idle_v, wait/20))
        else:
            asyncio.run(create_droplet_time(3, active_v, idle_v, wait/20))
        
        time.sleep(iteration_delay)
        
    #Stop all pumps
    set_all_pumps_voltage(0, 0, 0, 0)
    send_command("POFF")
    read_response(1)
        
    print("Sample program complete")
    
    connection.close()
    
