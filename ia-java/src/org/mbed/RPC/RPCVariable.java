package org.mbed.RPC;


/**
 *  This class is used to map a Java class on to the RPCVariable class. 
 *  This allows you to read and write to variables on mbed. This is entirely asynchronous so care may
 *  need to be taken to ensure that the mbed code and the software do not write to it at the same time.
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
public class RPCVariable<T>{
private MbedRPC the_mbed;
private String name;
	/**
	 * This constructor creates an instance which will control a ValuePass object which has already been created on the mbed
	 * <br>
	 * Note that it must also have the same type as the data on mbed or else data will be parsed incorrectly
	 * 
	 * @param connectedMbed The mbed object for the mbed that the ValuePass has been created on.
	 * @param ExistName The name of the existing ValuePass object
	 */
	public RPCVariable(MbedRPC connectedMbed, String ExistName){
		//Tie to existing instance
		the_mbed = connectedMbed;
		name = ExistName;
	}
	/**
	 * Write to a RPCVariable
	 * @param value The value that is to be written.
	 */
	public void write(T value){
		String s = String.valueOf(value);
		String[] Args = {s};
		the_mbed.RPC(name, "write", Args);
	}
	/**
	 * Reads back the current value of the RPCVariable and parses it as a float
	 * @return The value of the RPCVariable as a float
	 */
	public float read_float(){	
		String response = the_mbed.RPC(name, "read", null);
		//Need to convert response to a float and return
		if(response != null){
			float result = Float.parseFloat(response);
			return(result);
		}else{
			System.err.println("No string was returned to RPCVariable " + name + " . Value set as 0");
			return(0);
		}
	}
	/**
	 * Reads back the current value of the RPCVariable and parses it as a String
	 * @return The value of the RPCVariable as a String
	 */
	public String read_String(){	
		String response = the_mbed.RPC(name, "read", null);
		if(response != null){
			return(response);
		}else{
			System.err.println("No string was returned to RPCVariable " + name + " .");
			return(" ");
		}
	}
	/**
	 * Reads back the current value of the RPCVariable and parses it as an int
	 * @return The value of the RPCVariable as an int
	 */
	public int read_int(){	
		String response = the_mbed.RPC(name, "read", null);
		//Need to convert response to an int and return
		if(response != null){
			int result = Integer.parseInt(response);
			return(result);
		}else{
			System.err.println("No string was returned to RPCVariable " + name + " . Value set as 0");
			return(0);
		}
	}
	/**
	 * Reads back the current value of the RPCVariable and parses it as a char
	 * @return The value of the RPCVariable as a char
	 */
	public char read_char(){	
		String response = the_mbed.RPC(name, "read", null);
		//Need to convert response to a char and return
		if(response != null){
			char result = response.charAt(0);
			return(result);
		}else{
			System.err.println("No string was returned to RPCVariable " + name + " . Value set as 0");
			return(0);
		}
	}
	/**
	 * Reads back the current value of the RPCVariable and parses it as a double
	 * @return The value of the RPCVariable as a double
	 */
	public double read_Double(){	
		String response = the_mbed.RPC(name, "read", null);
		//Need to convert response to a float and return
		if(response != null){
			double result = Double.parseDouble(response);
			return(result);
		}else{
			System.err.println("No string was returned to RPCVariable " + name + " . Value set as 0");
			return(0);
		}
	}
}

