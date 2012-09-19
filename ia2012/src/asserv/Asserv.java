package asserv;
import mbed.SerialMbed;



public class Asserv
{
	private SerialMbed mbed;
	private String commande; // Dernière commande
	private boolean lastCommandFinished;

	public Asserv()
	{
		commande = "";
		try {
			mbed = new SerialMbed();
		} catch (Exception e) {
			e.printStackTrace();
		}
		lastCommandFinished = true;
	}

	public void gotoPosition(double x, double y)
	{
		commande = "g"+x+"#"+y+"!";
		sendCommand();
	}
	
	public void face(double x, double y)
	{
		commande = "f"+x+"#"+y+"!";
		sendCommand();
	}
	
	public void go(double d)
	{
		commande = "v"+d+"!";
		sendCommand();
	}
	
	public void turn(double a)
	{
		commande = "t"+a+"!";
		sendCommand();
	}
	
	public void halt()
	{
		try {
			mbed.send("h");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void resetHalt()
	{
		try {
			mbed.send("r");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void sendCommand()
	{
		System.out.println("sending : "+commande);
		try {
			synchronized (this) {
				mbed.send(commande);
				lastCommandFinished = false;
				launchFinishedChecker();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void eviteAdversaireDevant()
	{
		System.out.println("sending : "+commande);
		try {
			synchronized (this) 
			{
				resetHalt();
				mbed.send("v-50!");
				lastCommandFinished = false;
				launchFinishedChecker();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void eviteAdversaireDerriere()
	{
		System.out.println("sending : "+commande);
		try {
			synchronized (this) 
			{
				resetHalt();
				mbed.send("v50!");
				lastCommandFinished = false;
				launchFinishedChecker();
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

	private void launchFinishedChecker() {
		Thread checker = new Thread(new Runnable() {
			
			@Override
			public void run() {
				while(!lastCommandFinished) {
					//System.out.println("On check");
					if(mbed.ready()) {
						char check = mbed.getc();
						System.out.println("reçu : " + check);
						if(check == 'd') {
							lastCommandFinished = true;
						}
					} else {
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
			}
		});
		checker.start();
	}

	public void calageBordure(boolean sens) {
		try {
			mbed.send("c" + (sens ? "1" : "0") + "g");
			mbed.emptyBuffer();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public boolean lastCommandFinished() {
		return lastCommandFinished;
	}
	
}

