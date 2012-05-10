package asserv;
import java.util.ArrayList;

import mbed.SerialMbed;



public class Asserv
{
	private SerialMbed mbed;
	private ArrayList<String> list; // Liste des ordres
	private int current; // Dernier ordre donne a  la Mbed et atteint par le robot	

	public Asserv()
	{
		list = new ArrayList<String>();
		try {
			mbed = new SerialMbed();
		} catch (Exception e) {
			// TODO Auto-generated catch block
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
		list.add("h\n");
		sendLastCommand();
	}
	
	public void resetHalt()
	{
		list.add("r\n");
		sendLastCommand();
	}
	
	private void sendLastCommand()
	{
		try {
			mbed.send(list.get(list.size()-1));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public String refresh()
	{
		try {
			if(new String("d").equals(mbed.readLine()))
			{
				return list.get(current++);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
}

