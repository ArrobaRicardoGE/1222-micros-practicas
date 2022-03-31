import serial

ser = serial.Serial(
    port='COM1',
    baudrate=4800,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.EIGHTBITS)

print('Conected: ' + ser.portstr)

def to_volts(b):
    return int.from_bytes(b, 'big') / 255.0 * 5.0 

while True:
    b = ser.read()
    print(f'Measure: {to_volts(b):.2f}V (ADC: {int.from_bytes(b,"big")})')