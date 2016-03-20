package org.mbed.RPC;
/**
 *  This class is used to map a Java class on to the mbed API C++ class for AnalogIn.
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
public class MbedRPC {

	public static PinName LED1 = new PinName("LED1");
	public static PinName LED2 = new PinName("LED2");
	public static PinName LED3 = new PinName("LED3");
	public static PinName LED4 = new PinName("LED4");

	public static PinName p5 = new PinName("p5");
	public static PinName p6 = new PinName("p6");
	public static PinName p7 = new PinName("p7");
	public static PinName p8 = new PinName("p8");
	public static PinName p9 = new PinName("p9");
	public static PinName p10 = new PinName("p10");
	public static PinName p11 = new PinName("p11");
	public static PinName p12 = new PinName("p12");
	public static PinName p13 = new PinName("p13");
	public static PinName p14 = new PinName("p14");
	public static PinName p15 = new PinName("p15");
	public static PinName p16 = new PinName("p16");
	public static PinName p17 = new PinName("p17");
	public static PinName p18 = new PinName("p18");
	public static PinName p19 = new PinName("p19");
	public static PinName p20 = new PinName("p20");
	public static PinName p21 = new PinName("p21");
	public static PinName p22 = new PinName("p22");
	public static PinName p23 = new PinName("p23");
	public static PinName p24 = new PinName("p24");
	public static PinName p25 = new PinName("p25");
	public static PinName p26 = new PinName("p26");
	public static PinName p27 = new PinName("p27");
	public static PinName p28 = new PinName("p28");
	public static PinName p29 = new PinName("p29");
	public static PinName p30 = new PinName("p30");

	/**
     *
     * This method sends an RPC command and receives the response over the transport mechanisms that has been set up.
     * 
     *This method has been defined in the base class mbed but is overridden by the derived classes which actually implement the communication
     *
     * @param  Name	The Name of the object
     * @param  Method	The method of the object
     * @param  Args	An Array of the arguments which are to be passed 
     * @return Returns the response from the mbed RPC function.
     */
	public String RPC(String Name, String Method, String[] Args){
		//this should be over ridden by the derived class which inherits from this class
		String Arguments = "";
		int s = Args.length;
		for(int i = 0; i < s; i++){
			Arguments = Arguments + " " + Args[i];
		}
		System.out.println("Using Demo Mode: Construct mbed object using one of the derived classes such as MbedRPCSerial");
		//Print to console what would be written to mbed
		System.out.println("Output: /" + Name + "/" + Method +  Arguments);
		System.err.println("RPC not sent");
		return "" ;
	}

	/**
	 * This closes the transport mechanism. 
	 * 
	 */
	public void delete(){
		System.out.println("Either no transport mechanism in use or delete is not overidden by the transport mechanim in use.");
		this.delete();
	}
}
