package com.epam;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.SerialPort;
import javax.comm.UnsupportedCommOperationException;

public class GreyRead {
    private static final  char[]COMMAND = {'*', 'R', 'D', 'Y', '*'};
    private static final int WIDTH = 320; //640;
    private static final int HEIGHT = 240; //480;
    	
    private static CommPortIdentifier portId;
    InputStream inputStream;
    SerialPort serialPort;

    public static void main(String[] args) {
    	Enumeration portList = CommPortIdentifier.getPortIdentifiers();
        System.out.println("Hello");
        while (portList.hasMoreElements()) {
            System.out.println("Hello2");
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
            	System.out.println("Port name: " + portId.getName());
                if (portId.getName().equals("COM6")) {
                    GreyRead reader = new GreyRead();
                }
            }
        }
    }

    public GreyRead() {
       	int[][]rgb = new int[HEIGHT][WIDTH];
       	int[][]rgb2 = new int[HEIGHT][WIDTH];
    	
    	try {
            serialPort = (SerialPort) portId.open("SimpleReadApp", 1000);
            inputStream = serialPort.getInputStream();

            serialPort.setSerialPortParams(1000000,
                SerialPort.DATABITS_8,
                SerialPort.STOPBITS_1,
                SerialPort.PARITY_NONE);

        	int counter = 0;

        	while(true) {
        		System.out.println("Looking for image");
        	
        		int trial = 0;
        		while(!isImageStart(inputStream, 0)){
                            trial++;
                        };
                        
                        System.out.println("Dropped "  + trial + " bytes");
        	
	        	System.out.println("Found image: " + counter);
	        	
	        	for (int y = 0; y < HEIGHT; y++) {
	        		for (int x = 0; x < WIDTH; x++) {
		       			int temp = read(inputStream);
		    			rgb[y][x] = ((temp&0xFF) << 16) | ((temp&0xFF) << 8) | (temp&0xFF);
                                        // temp = read(inputStream);
	        		}
	        	}
	        	
	        	for (int y = 0; y < HEIGHT; y++) {
		        	for (int x = 0; x < WIDTH; x++) {
		        		rgb2[y][WIDTH - x - 1]=rgb[y][x];
		        	}	        		
	        	}
	        	
		        BMP bmp = new BMP();
	      		bmp.saveBMP("./out/out" + (counter++) + ".bmp", rgb2);
	      		
	      		System.out.println("Saved image: " + counter);
        	}
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
    
    private int read(InputStream inputStream) throws IOException {
    	int temp = (char) inputStream.read();
		if (temp == -1) {
			throw new  IllegalStateException("Exit");
		}
		return temp;
    }
    	
    private boolean isImageStart(InputStream inputStream, int index) throws IOException {
    	if (index < COMMAND.length) {
    		if (COMMAND[index] == read(inputStream)) {
    			return isImageStart(inputStream, ++index);
    		} else {
    			return false;
    		}
    	}
    	return true;
    }
}