package asserv;
import java.util.ArrayList;

import mbed.SerialMbed;



public class Asserv
{
	private SerialMbed mbed;
	private ArrayList<String> list; // Liste des ordres
	private int current; // Dernier ordre donne a la Mbed et atteint par le robot	

	public Asserv()
	{
		list = new ArrayList<String>();
		try {
			mbed = new SerialMbed();
		} catch (Exception e) {
			e.printStackTrace();
		}
		current = 0;
	}

	public void gotoPosition(double x, double y)
	{
		list.add("g"+x+"#"+y+"\n");
		sendLastCommand();
	}
	
	public void face(double x, double y)
	{
		list.add("f"+x+"#"+y+"\n");
		sendLastCommand();
	}
	
	public void go(double d)
	{
		list.add("v"+d+"\n");
		sendLastCommand();
	}
	
	public void turn(double a)
	{
		list.add("t"+a+"\n");
		sendLastCommand();
	}
	
	public void halt()
	{
		list.add("h");
		sendLastCommand();
	}
	
	public void resetHalt()
	{
		list.add("r");
		sendLastCommand();
	}
	
	public void sendLastCommand()
	{
		try {
			if(current == list.size()-1) {
				mbed.send(list.get(current));
				System.out.println("'" + list.get(current) + "'");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/*
	public String refresh()
	{
		try {
			if(new String("d").equals(mbed.readLine()))
			{
				return list.get(current++);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	*/

	public void calageBordure(boolean sens) {
		try {
			mbed.send("c" + (sens ? "1" : "0") + "g");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void emptyReceptionBuffer() {
		while(mbed.ready()) {
			mbed.skip();
		}
	}

	public boolean lastCommandFinished() {
		if(mbed.ready()) {
			char recu = mbed.getc();
			System.out.println("recu : '"+recu+"'");
			if(recu=='d') {
				current++;
				return true;
			}
		}
		return false;
	}
	
}

