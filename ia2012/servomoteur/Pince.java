package servomoteur;

public class Pince {
	PololuMicroMaestro p;
	
	public Pince(){
		p = new PololuMicroMaestro("/dev/ttyACM0", "/dev/ttyACM1", 12);
		p.setMinPosms(1,1357);
		p.setMaxPosms(1,2450);
		p.setMinPosms(2,928);
		p.setMaxPosms(2,1804);
	}
	
	public void openPinceAv(){
			p.setPositionms(1,p.getMaxPosms(1));
			p.setPositionms(2,p.getMinPosms(2));
			
		}
		
	public void closePinceAv(){
			p.setPositionms(1,p.getMinPosms(1));
			p.setPositionms(2,p.getMaxPosms(2));
			
		}
		
	public void setSpeed(int speed){ // MARCHE PAS !
		p.setmaxSpeed(1,speed);
		p.setmaxSpeed(2,speed);
		
	}

	

}

