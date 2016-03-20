package org.mbed.RPC;


/**
 *  This class is used to retrieve the objects that can be created on mbed
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
public class GetObjects {
	private MbedRPC the_mbed;

	public String[] ObjArray = null;
	public String[] MethodsArray = null;
	
	/**
	 * Create a GetObjects Object for mbed
	 * 
	 * @param connectedMbed The mbed object for the mbed to read the objects from
	 */
		public GetObjects(MbedRPC connectedMbed) {
			//Create a new DigitalOut on mbed
			the_mbed = connectedMbed;
			Objects();
		}
	/**
	 * Read a list of all the objects that can be accessed using the RPC.
	 * 
	 * @return A string array of objects
	 */
	public String[] Objects(){
		String response;
		response = the_mbed.RPC(" ", null, null);
		
		ObjArray = null;
		//parse response by spaces to and transfer object names into array	
		ObjArray = response.split(" ");
		
		return(ObjArray);
	}
	/**
	 * Return the methods accessible over RPC on this object.
	 * @param ObjName The name of an object which is accessible over RPC
	 * @return A string array of the methods accessible on this object
	 */
	public String[] methods(String ObjName){
		String response;
		response = the_mbed.RPC(ObjName, " ", null);
		MethodsArray = null;
		//parse response by spaces to and transfer method names into array
		MethodsArray  = response.split(" ");
		return(MethodsArray);
	}
	
}
