import sys
import glob
import serial
import os
import io
from PIL import Image
from array import array



class SerialImageReader():
    def __init__(self):
        self.imsize = 320*240
        self.bytes = [0]*self.imsize
        
    def showImage(self, path):
        count = os.stat(path).st_size / 2
        
        #with open(path, "rb") as f:
            #bytes = bytearray(f.read(imsize))
        width = 320
        height = 240
        
        print("Bytes read from file:", len(bytes))
        img = Image.new('RGB', (width, height), "white")
        pixels = img.load()
        
        for i in range(height):
            offset = i*width
            for j in range(width):
                y = bytes[offset + j]
                pixels[j, i] = (y, y, y)

        img.show()

    def serial_ports(self):
        """ Lists serial port names

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result

    def isEqual(self, a, b):
        for i in range(0, len(a)):
            if a[i]!= b[i]:
                print(a[i], b[i])
                return False
        return True

    def pattern_search(self):
        count = 0
        with open('data.bin', 'rb') as f:
            b = f.read()
            for i in range(0, len(b) - 3):
                if b[i:i+5] == b'*RDY*':
                    print(i)
                    count = count+1
                    
            print(count, ' * found')
            

    def serial_read_img(self):
        print(self.serial_ports())
        keepTaking = True
        while keepTaking:
            #keepTaking = False
            with serial.Serial('COM6', 1000000, parity=serial.PARITY_NONE) as ser:
                with open('data.bin', 'wb') as bf:
                    i = 0
                    print(ser.read(5))
                    
                    data_len = 320*240 # 2 bytes per pixel on average
                    print(data_len)
                    #bf.write(ser.read(data_len))
                    self.bytes = ser.read(data_len)
                    
                    #print(ser.read(5))
                    '''
                    while not i > data_len:
                        i = i + 100
                        
                        if(i%2000 == 0):
                            print(i)
                            
                    print(ser.read(5))
                    '''
                    
                showImage('data.bin')
    def run(self):
        self.serial_read_img()
                    
if __name__ == '__main__':
    siReader = SerialImageReader()
    siReader.run()
    
    #readimage('data.bin')
    #pattern_search()
