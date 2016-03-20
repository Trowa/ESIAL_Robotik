package org.mbed.RPC;


/**
 *  This class is used to map a Java class on to the mbed API C++ class for PwmOut.
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
public class PwmOut{
	private MbedRPC the_mbed;
	private String name;
		public PwmOut(MbedRPC connectedMbed, PinName pin) {
			//Create a new PwmOut on mbed
			the_mbed = connectedMbed;
			String[] Args = {pin.ident, "pwm_out_" + pin.ident };
			name = the_mbed.RPC("PwmOut", "new", Args);
			System.out.println("PwmOut Received Name: " + name);
		}
		public PwmOut(MbedRPC connectedMbed, String ExistName){
			//Tie to existing instance
			the_mbed = connectedMbed;
			name = ExistName;
		}
		public void write(double value){
			String s = String.valueOf(value);
			String[] Args = {s};
			the_mbed.RPC(name, "write", Args);
		}
		public float read(){
			String response = the_mbed.RPC(name, "read", null);
			//Need to convert response to a float and return
			float f = Float.parseFloat(response);
			return(f);
		}
		public void period(double value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "period", Args);
		}
		public void period_ms(int value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "period_ms", Args);
		}
		public void period_us(int value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "period_us", Args);
		}
		public void pulsewidth(double value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "pulsewidth", Args);

		}
		public void pulsewidth_ms(int value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "pulsewidth_ms", Args);
		}
		public void pulsewidth_us(int value){
			String[] Args = {String.valueOf(value)};
			the_mbed.RPC(name, "pulsewidth_us", Args);
		}
		public void delete(){
			the_mbed.RPC(name, "delete", null);
		}
	}