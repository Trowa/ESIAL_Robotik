package servomoteur;

public class Pince {
	PololuMicroMaestro p;
	
	public Pince(){
		p = new PololuMicroMaestro("/dev/ttyACM0", "/dev/ttyACM1", 12);
		p.setMinPosms(1,1357);// Pince AVG
		//p.setMaxPosms(1,2450);
		p.setMaxPosms(1,2142);
		//p.setMinPosms(2,928);  // Pince AVD
		p.setMinPosms(2,1025);
		p.setMaxPosms(2,1804);
		//p.setMinPosms(3,992); //Pince ARG
		p.setMinPosms(3,1118);
		p.setMaxPosms(3,2000);
		p.setMinPosms(4,800); //Pince ARD
		//p.setMaxPosms(4,2000);
		p.setMaxPosms(4,1715);
		p.setMinPosms(5,1050);
		p.setMaxPosms(5,1740);
	}
	
	public void openPinceAv(){
		p.setPositionms(1,p.getMaxPosms(1));
		p.setPositionms(2,p.getMinPosms(2));
	}
		
	public void closePinceAv(){
		p.setPositionms(1,p.getMinPosms(1));
		try{
			Thread.sleep(500);
		} catch (InterruptedException e) {
		}
		p.setPositionms(2,p.getMaxPosms(2));
	}
		
	public void openPinceAr(){
		p.setPositionms(3,p.getMinPosms(3));
		p.setPositionms(4,p.getMaxPosms(4));
			
	}
		
	public void closePinceAr(){
		p.setPositionms(3,p.getMaxPosms(3));
		
		try{
			Thread.sleep(500);
		} catch (InterruptedException e) {
		}

		p.setPositionms(4,p.getMinPosms(4));
	}
	
	public void setSpeed(int speed){ // MARCHE PAS !
		p.setmaxSpeed(1,speed);
		p.setmaxSpeed(2,speed);
	}

	public void ejecter(){
		p.setPositionms(5,p.getMaxPosms(5));
	}
	
	public void fermerBenne(){
		p.setPositionms(5,p.getMinPosms(5));
	}
	

}

