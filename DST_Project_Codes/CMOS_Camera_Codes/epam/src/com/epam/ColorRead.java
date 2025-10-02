package com.epam;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;

import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.SerialPort;
import javax.comm.UnsupportedCommOperationException;

public class ColorRead {
    private static final  char[]COMMAND = {'*', 'R', 'D', 'Y', '*'};
    private static final int WIDTH = 320; //640;
    private static final int HEIGHT = 240; //480;
    	
    private static CommPortIdentifier portId;
    InputStream inputStream;
    SerialPort serialPort;

    public static void main(String[] args) {
    	Enumeration portList = CommPortIdentifier.getPortIdentifiers();
        System.out.println("Color Read :) ");
        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
            	System.out.println("Port name: " + portId.getName());
                if (portId.getName().equals("COM6")) {
                    ColorRead reader = new ColorRead();
                }
            }
        }
    }
    int YCbCr2RGB(int y, int cb, int cr){
        int cr2 = cr - 128;
        int cb2 = cb - 128;
        int r = new Double(y + 1.402*cr2).intValue();
        int g = new Double(y - 0.34414*cb2 - 0.71414*cr2).intValue();
        int b = new Double(y + 1.772*cb2).intValue();
        
        return ((g & 0xFF) << 16) | ((r & 0xFF) << 8) | (b & 0xFF);
//        int r = cb;
//        return ((r&0xFF) << 16) | ((r&0xFF) << 8) | (r&0xFF);
    }
    public ColorRead() {
       	int[][][]rgb = new int[HEIGHT][WIDTH][2];
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
	        	System.out.println("Found image: "  + counter);
	        	
	        	for (int y = 0; y < HEIGHT; y++) {
	        		for (int x = 0; x < WIDTH; x++) {
                                    for(int z =0; z<2; z++){
                                        int temp = read(inputStream);
		    			//rgb[y][x][0] = ((temp&0xFF) << 16) | ((temp&0xFF) << 8) | (temp&0xFF);
                                        rgb[y][x][z] = temp;
                                    }
	        		}
	        	}
	        	
	        	for (int y = 0; y < HEIGHT; y++) {
		        	for (int x = 0; x < WIDTH; x+=2) {
                                        int y0 = rgb[y][x][0];
                                        int cb = rgb[y][x][1];
                                        int y1 = rgb[y][x + 1][0];
                                        int cr = rgb[y][x + 1][1];
		        		rgb2[y][x] = YCbCr2RGB(y0, cb, cr);
                                        rgb2[y][x + 1] = YCbCr2RGB(y1, cb, cr);
//                                        int temp = y0;
//                                        rgb2[y][x] = ((temp&0xFF) << 16) | ((temp&0xFF) << 8) | (temp&0xFF);
//                                        temp = y1;
//                                        rgb2[y][x + 1] = ((temp&0xFF) << 16) | ((temp&0xFF) << 8) | (temp&0xFF);
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