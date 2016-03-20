package org.mbed.RPC;


/**
 *  This class is used to map a Java class on to the mbed API C++ class for Serial.
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
public class Serial{
	private MbedRPC the_mbed;
	private String name;
		public Serial(MbedRPC connectedMbed, PinName TxPin, PinName RxPin) {
			//Create a new Serial on mbed
			the_mbed = connectedMbed;
			String[] Args = {TxPin.ident, RxPin.ident, "serial_" + TxPin.ident + "_" + RxPin.ident};
			name = the_mbed.RPC("Serial", "new", Args);
			System.out.println("Serial Received Name: " + name);
		}
		public Serial(MbedRPC connectedMbed, String ExistName){
			//Tie to existing instance
			the_mbed = connectedMbed;
			name = ExistName;
		}
		public void baud(int baudRate){
			String[] Args = {String.valueOf(baudRate)};
			the_mbed.RPC(name, "baud", Args);
		}
		public void format(int bits, String Parity, int stop_bits){
			String[] Args = {String.valueOf(bits) , Parity , String.valueOf(stop_bits) };
			the_mbed.RPC(name, "format", Args);
		}
		public void putc(char c){
			String[] Args = {String.valueOf((int)c)};
			the_mbed.RPC(name, "putc", Args);
		}
		public char getc(){
			String response = the_mbed.RPC(name, "getc", null);
			int i = Integer.parseInt(response);
			char c = (char)i;
			return(c);
		}
		public String gets(){
			String rxString = null;
			int i = 0;
			char c;
			do{
				c = getc();
				//rxString.concat(Character.toString(c));
				rxString = rxString + c;
				i++;
			}while(rxString.charAt(i - 1) != (char)10);
			return(rxString);
		}
		public void puts(String data){
			char c;
			for(int i = 0; i < data.length(); i++){
				c = data.charAt(i);
				putc(c);
			}
			putc('\n');
		}
		public boolean readable(){
			String result = the_mbed.RPC(name, "readable", null);
			//use correct form depending on how mbed responds
			boolean readable = (Integer.parseInt(result) != 0);
			//boolean readable = Boolean.parseBoolean(result);
			return(readable);
		}
		public boolean writeable(){
			String result = the_mbed.RPC(name, "writeable", null);
			boolean writeable = (Integer.parseInt(result) != 0);
			//boolean writeable = Boolean.parseBoolean(result)
			return(writeable);
		}
		public void delete(){
			the_mbed.RPC(name, "delete", null);
		}
}

