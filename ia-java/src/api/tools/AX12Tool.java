package api.tools;

import fr.nancyborg.ax12.AX12;

public class AX12Tool {
	public static void main(String[] args) {
		int id = Integer.parseInt(args[0]);
		
		AX12 ax = new AX12("/dev/ttyAMA0", id, 115200);		
		System.out.println("wait...");
		ax.pingWait();
		System.out.println("ok");
		
		if (args.length > 1) {
			ax.setGoalPosition(Integer.parseInt(args[1]), true);
		}
		else {
			for (;;) {
				System.out.println("pos: " + ax.getPresentPosition());
			}
		}
	}
}
