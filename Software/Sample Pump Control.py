import serial
import serial.tools.list_ports
import time
import asyncio
import nest_asyncio

HARDWARE_ID = "2341:8037"
DEVICE_ID = "PocketLoCPumpController"
LINE_TERMINATOR = "\r\n"
BAUDRATE = 9600
TIMEOUT = 1 #s

connection = None

def connect_serial():
    ports = serial.tools.list_ports.comports()
    
    conn = None
    for port, desc, hwid in sorted(ports):
        if HARDWARE_ID in hwid:            
            try:
                conn = serial.Serial(port=port, baudrate=BAUDRATE, timeout=TIMEOUT)
            except serial.serialutil.SerialException:
                #Port may be in use
                print("Port in use. This may be due to an aborted connection. If no other matching port is found try un- and replugging the device.")
                continue
            
            conn.write(str.encode("ID" + LINE_TERMINATOR))
            read_id = conn.readline().decode('utf-8').replace(LINE_TERMINATOR, "")
            if read_id == DEVICE_ID:
                break
            else:
                conn.close()
                conn = None
            
    
    if conn is None or not conn.is_open:
        raise Exception("Could not find valid device to connect to.")
    
    return conn

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
    await asyncio.sleep(duration)
    set_pump_voltage(pump, off_voltage)

async def create_droplet_mix(max_time, pump2_factor, pump3_factor, pump4_factor, pump3_wait, pump4_wait, on_voltage, off_voltage, delay):
    #Create a droplet as a mix of three pump actuations with a maximal duration of max_time
    #pumpN_factor sets the individual pump duration (max_time*pumpN_factor)
    #pump N is delayed by pumpN_wait [ms] (serially)
    #delay [s] is applied as a total duration and must be larger than any pump actuation time
    
    d_task = asyncio.create_task(asyncio.sleep(delay))
    
    t1 = asyncio.create_task(create_droplet_time(2, on_voltage, off_voltage, max_time*pump2_factor))
        
    await asyncio.sleep(pump3_wait/1000)

    t2 = asyncio.create_task(create_droplet_time(3, on_voltage, off_voltage, max_time*pump3_factor))
        
    await asyncio.sleep(pump4_wait/1000)
    
    t3 = asyncio.create_task(create_droplet_time(4, on_voltage, off_voltage, max_time*pump4_factor))
        
    await d_task
    await t1
    await t2
    await t3
    

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
    send_command("PON")
    read_response(1)
    
async def timed_samples(active_v, idle_v, iteration_delay):
    #Run some samples async to get a feeling for parallel timed droplets
    
    #Mixed droplets - fading from pump 2 to 3
    print("Producing 10 droplets, fading from mostly pump 2 to mostly pump 3.")
    for wait in range(1, 11):
        d_task = asyncio.create_task(asyncio.sleep(iteration_delay))
        
        t1 = asyncio.create_task(create_droplet_time(2, active_v, idle_v, (10-wait)/20))
        t2 = asyncio.create_task(create_droplet_time(3, active_v, idle_v, wait/20))
        
        await d_task
        await t1
        await t2
                
    #Mixed droplet - all three pumps using easy script
    #max time per pump = 0.5s, *1 for all three pumps, 100ms wait between pumps
    print("Creating a coherent droplet.")
    await create_droplet_mix(0.5, 1, 1, 1, 100, 100, active_v, idle_v, iteration_delay)
    
    
if __name__ == '__main__':
    nest_asyncio.apply() #Needed for IPython instances
    
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
    #synchronous stuff
    for pump in range(2, 5):
        print("Producing 5 droplets with increasing size with pump " + str(pump))
        for wait in range(1, 6):
            set_pump_voltage(pump, active_v)
            time.sleep(wait/10)
            set_pump_voltage(pump, idle_v)
            time.sleep(iteration_delay)
          
    #Lets have a look at some asychronous samples
    asyncio.run(timed_samples(active_v, idle_v, iteration_delay))
    
        
    #Stop all pumps
    set_all_pumps_voltage(0, 0, 0, 0)
    send_command("POFF")
    read_response(1)
        
    print("Sample program complete")
    
    connection.close()
    
