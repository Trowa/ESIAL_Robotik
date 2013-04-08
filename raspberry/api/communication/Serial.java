package api.communication;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class Serial {

	private OutputStream deviceOut;
	private FileInputStream deviceIn;
	
	public Serial(String path) throws IOException {
		
		File file = null;
		
		try {
			file = new File(path);
			deviceIn = new FileInputStream(file);
			deviceOut = new FileOutputStream(file);
			Runtime.getRuntime().exec("stty -F " + file.getAbsolutePath() + " ispeed 115200 ospeed 115200");
		} catch(FileNotFoundException e) {
			e.printStackTrace();
		}
		System.out.println("Serial sur "+file.getAbsolutePath());
	}
	
	public char getChar() {
		try {
			return (char) deviceIn.read();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return 0;
	}
	
	public boolean ready() {
		try {
			return deviceIn.available()>0;
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
		
	public void send(String s) throws Exception {
		deviceOut.write(s.getBytes());
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
