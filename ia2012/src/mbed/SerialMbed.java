package mbed;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;


public class SerialMbed {
	private OutputStream deviceOut;
	private BufferedReader deviceIn;

	public SerialMbed() throws Exception {
		deviceIn = new BufferedReader(new InputStreamReader(new FileInputStream(new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01"))));
		deviceOut = new FileOutputStream(new File("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F09785-if01"));
	}

	public String readLine() throws Exception {
		return deviceIn.readLine();
	}

	public void send(String s) throws Exception
	{
		deviceOut.write(s.getBytes());
		deviceOut.flush();
	}
}

