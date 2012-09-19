package mbed;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;


public class SerialMbed {
	private OutputStream deviceOut;
	private FileInputStream deviceIn;
	
	/*
	 * Pour l'instant, les deux fichiers à tester sont :
	 * /dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F04F94-if01
	 * /dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01
	 * 
	 * Correspondent aux 2 Mbed qu'on a
	 */

	public SerialMbed() throws IOException {
		
		File mbedFile;
		
		try {
			mbedFile = new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01");
			if(!mbedFile.exists()) {
				throw new FileNotFoundException();
			}
			deviceIn = new FileInputStream(mbedFile);
			deviceOut = new FileOutputStream(mbedFile);
			Runtime.getRuntime().exec("stty -F " + mbedFile.getAbsolutePath() + " ispeed 115200 ospeed 115200");
		} catch(FileNotFoundException e) {
			mbedFile = new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F04F94-if01");
			if(!mbedFile.exists()) {
				throw new FileNotFoundException();
			}
			deviceIn = new FileInputStream(mbedFile);
			deviceOut = new FileOutputStream(mbedFile);
			Runtime.getRuntime().exec("stty -F " + mbedFile.getAbsolutePath() + " ispeed 115200 ospeed 115200");
		}
		//System.out.println("Mbed sur "+pandaFile.getAbsolutePath());
	}
	
	/*
	public String readLine() throws Exception {
		return deviceIn.readLine();
	}
	*/
	
	public char getc() {
		try {
			return (char) deviceIn.read();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return 0;
	}
	
	public boolean ready() {
		try {
			return deviceIn.available()>0;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
		
	public void send(String s) throws Exception
	{
		deviceOut.write(s.getBytes());
		//System.out.println("envoyé : '" + s + "'");
		deviceOut.flush();
		System.out.println("flush");
		
	}

	public void emptyBuffer() {
		while(ready()) {
			try {
				deviceIn.read();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	

}

