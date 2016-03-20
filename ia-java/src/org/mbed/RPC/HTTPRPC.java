package org.mbed.RPC;

import java.net.*;
import java.security.AccessControlException;
import java.io.*;
import java.applet.Applet;
/**
 *  This class is used to create an object which can communicate using RPC to an mbed Connected over HTTP
 *  
 * @author Michael Walker
 * @license
 * Copyright (c) 2010 ARM Ltd
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 * <br>
 *The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 * <br>
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 *
 */
public class HTTPRPC extends MbedRPC {
URL mbedAddr = null;
public String Address;
URLConnection mbedConnection = null;
BufferedReader from_mbed = null;
/**
 * Constructor to create an mbed object to control an mbed over HTTP.
 * @param Addr the address of the mbed eg "http://192.168.2.2"
 */
	public HTTPRPC(String Addr){
		Address = Addr;
	}
	/**
	 *  Constructor to create an mbed object to control an mbed over HTTP. 
	 *  This constructor is for use when using an applet hosted on mbed.
	 * @param mbedApplet The Applet that is hosted on the mbed to be controlled
	 */
	public HTTPRPC(Applet mbedApplet){
		URL mbedAddress = mbedApplet.getCodeBase();
		if(mbedAddress != null) {
			Address = "http://" + mbedAddress.getHost();
			System.out.println(Address);
		}else{
			System.err.println("Error retireving address of mbed");
			Address = "error";
		}
	}
	/**
	 * {@inheritDoc}
	 */
	public String RPC(String Name, String Method, String[] Args){
		//Execute RPC GET command and get result back
		String Response;
		String Arguments = "";
		if(Args != null){
			int s = Args.length;
			for(int i = 0; i < s; i++){
				Arguments = Arguments + "%20" + Args[i];
			}
		}
		
		String Command = "/rpc/" + Name + "/" + Method + Arguments;
		System.out.println(Address + Command);
		try{
			mbedAddr = new URL(Address + Command);
	        BufferedReader from_mbed = new BufferedReader(new InputStreamReader(mbedAddr.openStream()));
	        
	       if(from_mbed.ready() == true){
	    	   Response = from_mbed .readLine();
	    	   //System.out.println(Response);
	       }else{
	    	   Response = "empty";
	       }
	       from_mbed.close();
		}catch(MalformedURLException e){
			Response = "error";
			System.err.println("Malformed URL Exception");
		}catch(IOException e){
			Response = "Error";
			System.err.println("IO Exception in RPC");
		}catch(AccessControlException e){
			Response = "Error";
			System.err.println("Applet is not running on server it is communicating with - AccessControlException");
		}catch(NullPointerException e){
			Response = "Error";
		}
		
		return(Response);
	}
	/**
	 * {@inheritDoc}
	 */
	public void delete(){
		//close the HTTP connection
		try{
			from_mbed.close();
		}catch(IOException e){
			System.err.println("IO Exception in delete");
		}catch(NullPointerException e){
			
		}
	}
}
