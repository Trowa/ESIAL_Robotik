package servomoteur;




import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.File;

public class PololuMicroMaestro {

	private byte deviceNumber;
	private OutputStream deviceFile;
	private int[] minPosms;
	private int[] maxPosms;

	public PololuMicroMaestro(String deviceFileName, String deviceFileReceiverName, int deviceNumber) {
		this.deviceNumber = (byte) deviceNumber;
		try {
			deviceFile = new FileOutputStream(new File(deviceFileName));
		} catch(Exception e) {
			e.printStackTrace();
		}
		minPosms=new int[6];
		maxPosms=new int[6];
		for(int i = 0;i<6;i++){
			minPosms[i] = 500;
			maxPosms[i] = 2400;
		}
	}

	public void setMinPosms(int channel, int ms) {
		minPosms[channel] = ms;
	}
	
	public int getMinPosms(int channel) {
		return minPosms[channel];
	}

	public void setMaxPosms(int channel,int ms) {
		maxPosms[channel] = ms;
	}
	public int getMaxPosms(int channel) {
		return maxPosms[channel];
	}

	public void setPositionms(int channel, int ms) {

		if(ms < minPosms[channel]) {
			ms = minPosms[channel];
		} else if(ms > maxPosms[channel]) {
			ms = maxPosms[channel];
		}

		byte[] buffer = new byte[6];
		buffer[0] = (byte) 0xAA;
		buffer[1] = deviceNumber;
		buffer[2] = (byte) 0x04;
		buffer[3] = (byte) channel;
		buffer[4] = (byte) ((4*ms) % 128);
		buffer[5] = (byte) ((4*ms) / 128);

		try {
			deviceFile.write(buffer);
			deviceFile.flush();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	/*
	public int getPositionms(int channel) {
		byte[] buffer = new byte[4];
		buffer[0] = (byte) 0xAA;
		buffer[1] = deviceNumber;
		buffer[2] = (byte) 0x10;
		buffer[3] = (byte) channel;
		try {
			deviceFile.write(buffer);
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		return 0;
	}
	*/
	
	public void setmaxSpeed(int channel, int speed) {
		//0xAA, device number, 0x07, channel number, speed low bits, speed high bits
		

		byte[] buffer = new byte[6];
		buffer[0] = (byte) 0xAA;
		buffer[1] = deviceNumber;
		buffer[2] = (byte) 0x07;
		buffer[3] = (byte) channel;
		buffer[4] = (byte) ((speed) % 128);
		buffer[5] = (byte) ((speed) / 128);

		try {
			deviceFile.write(buffer);
			deviceFile.flush();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
}
