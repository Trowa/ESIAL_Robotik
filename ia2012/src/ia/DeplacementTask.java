package ia;

import asserv.Asserv;

public class DeplacementTask implements Runnable {
	
	private Asserv asservissement;
	private boolean sens;
	
	public DeplacementTask(Asserv asservissement, boolean sens) {
		this.asservissement = asservissement;
		this.sens = sens;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		
		
		
		
		try {
			Thread.sleep(1000000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
