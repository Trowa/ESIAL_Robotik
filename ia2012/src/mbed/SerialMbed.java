package mbed;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;


public class SerialMbed {
	private OutputStream deviceOut;
	//private BufferedReader deviceIn;
	
	/*
	 * Pour l'instant, les deux fichiers à tester sont :
	 * /dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F04F94-if01
	 * /dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01
	 * 
	 * Correspondent aux 2 Mbed qu'on a
	 */

	public SerialMbed() throws IOException {
		
		File pandaFile;
		
		try {
			pandaFile = new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01");
			if(!pandaFile.exists()) {
				throw new FileNotFoundException();
			}
			//deviceIn = new BufferedReader(new InputStreamReader(new FileInputStream(pandaFile)));
			deviceOut = new FileOutputStream(pandaFile);
			Process child = Runtime.getRuntime().exec("stty -F " + pandaFile.getAbsolutePath() + " ispeed 115200 ospeed 115200");
		} catch(FileNotFoundException e) {
			pandaFile = new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F04F94-if01");
			if(!pandaFile.exists()) {
				throw new FileNotFoundException();
			}
			//deviceIn = new BufferedReader(new InputStreamReader(new FileInputStream(pandaFile)));
			deviceOut = new FileOutputStream(pandaFile);
			Process child = Runtime.getRuntime().exec("stty -F " + pandaFile.getAbsolutePath() + " ispeed 115200 ospeed 115200");
		}
		System.out.println("Mbed sur "+pandaFile.getAbsolutePath());
	}
	
	/*
	public String readLine() throws Exception {
		return deviceIn.readLine();
	}
	
	public char getc() throws IOException {
		return (char) deviceIn.read();
	}
	*/
	
	public void send(String s) throws Exception
	{
		deviceOut.write(s.getBytes());
		System.out.println("'" + s + "'");
		deviceOut.flush();
	}
}

