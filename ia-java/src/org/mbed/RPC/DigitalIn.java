package org.mbed.RPC;


/**
 *  This class is used to map a Java class on to the mbed API C++ class for DigitalIn.
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
public class DigitalIn{
	private MbedRPC the_mbed;
	private String name;
		public DigitalIn(MbedRPC connectedMbed, PinName pin) {
			//Create a new DigitalIn on mbed
			the_mbed = connectedMbed;
			String[] Args = {pin.ident, "digital_in_" + pin.ident };
			name = the_mbed.RPC("DigitalIn", "new", Args);
			System.out.println("DigitalIn Received Name: " + name);
		}
		public DigitalIn(MbedRPC connectedMbed, String ExistName){
			//Tie to existing instance
			the_mbed = connectedMbed;
			name = ExistName;
		}
		public int read(){
			String response = the_mbed.RPC(name, "read", null);
			//Need to convert response to and int and return
			int i = Integer.parseInt(response);
			return(i);
		}
		public void delete(){
			the_mbed.RPC(name, "delete", null);
		}
	}
