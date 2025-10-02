package com.epam;

import javax.comm.*;
import java.util.*;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/** List all the ports available on the local machine. **/
public class testSerial
{
    public static void main (String args[])
    {

        Enumeration port_list = CommPortIdentifier.getPortIdentifiers();        
        

        while (port_list.hasMoreElements())
        {
            System.out.println("YES");
            CommPortIdentifier port_id = (CommPortIdentifier)port_list.nextElement();

            if (port_id.getPortType() == CommPortIdentifier.PORT_SERIAL)
            {
                System.out.println ("Serial port: " + port_id.getName());
            }
            else if (port_id.getPortType() == CommPortIdentifier.PORT_PARALLEL)
            {
                System.out.println ("Parallel port: " + port_id.getName());
            }
            else
                System.out.println ("Other port: " + port_id.getName());
        }
    } // main
} // class PortList