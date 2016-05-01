package ia.esialrobotik.objectifs;

import java.util.ArrayList;
import java.util.List;

import api.asserv.actions.Action;
import api.asserv.actions.Go;
import api.asserv.actions.WaitAndCancel;
import ia.common.Objectif;
import navigation.Point;

/**
 * Coupe 2016
 * Une cabine de plage: une simple porte à pousser avec le robot
 */
public class Cabine extends Objectif {
	
	public Cabine(Point position) {
		super(position);
	}

	@Override
	protected List<Action> generateListActions() {
		List<Action> actions = new ArrayList<Action>();
		
		// On suppose que l'on est en face de la cabine, on se contente de pousser et de reculer
		actions.add(new Go(150));
		actions.add(new WaitAndCancel(1500));
		actions.add(new Go(-150));
		
		return actions;
	}


}
