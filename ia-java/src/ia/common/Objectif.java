package ia.common;

import java.util.List;

import api.asserv.actions.Action;
import navigation.Point;

/**
 * Created by mickael on 13/05/15.
 */
public abstract class Objectif {
	
	private boolean done; // L'objectif est-il rempli ?
	private Point position; // La position à atteindre pour traiter l'objectif
	private List<Action> actions; // Les actions à effectuer pour remplir l'objectif

	/**
	 * Un nouvel objectif
	 * @param position La position à atteindre pour traiter l'objectif
	 * @param actions Les actions à effectuer pour remplir l'objectif
	 */
	protected Objectif(Point position) {
		this.position = position;
		this.actions = generateListActions();
		this.done = false;
	}

	/**
	 * L'objectif est-il rempli ?
	 * @return true si l'objectif a déjà été rempli
	 */
	public boolean isDone() {
		return done;
	}
	
	/**
	 * Indique que l'objectif a été rempli
	 */
	public void setDone() {
		done = true;
	}

	/**
	 * Revoie la position et le cap à atteindre pour traiter l'objectif
	 * @return La position et le cap à atteindre
	 */
	public Point getPosition() {
		return position;
	};
	
	/**
	 * Renvoie les actions à effectuer pour remplir l'objectif
	 * @return la liste des actions
	 */
	public List<Action> getActions() {
		return actions;
	}

	/**
	 * Construit la liste des actions. A implémenter dans une sous-classe
	 * @return la liste des actions
	 */
	protected abstract List<Action> generateListActions();
}
