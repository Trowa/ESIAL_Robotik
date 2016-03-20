package navigation;

import java.util.ArrayList;

/**
 * Interpolateur linéaire utilisant la méthode des moindres carrées pour calculer
 * la droite d'interpolation
 * @author Jean-Baptiste HERVÉ
 *
 */
public class RegressionLineaire {

  private ArrayList<Point> data;
  
  // Données de sortie : beta0 et beta1 ( interpolation y = beta1 * x + beta0 )
  private double beta0;
  private double beta1;
  private double correlCoeff;
  
  // Données de calcul
  private double sommeX;
  private double sommeY;
  
  private double xBarre;
  private double yBarre;
  
  private double xxBarre;
  private double yyBarre;
  private double xyBarre;
  
  /**
   * Instancie un interpolateur linéaire
   */
  public RegressionLineaire() {
    data = new ArrayList<Point>();
    beta0 = 0.0;
    beta1 = 0.0;
    correlCoeff = 0.0;
    sommeX = 0;
    sommeY = 0;
  }
  
  /**
   * Paramétrage de l'ensemble des points à interpoler
   * @param data Une liste de points
   */
  public void setData(ArrayList<Point> data) {
    this.data.clear();
    this.data.addAll(data);
    performComputation();
  }
  
  /**
   * Ajout d'un point à interpoler
   * @param p Le point à ajouter
   */
  public void addPoint(Point p) {
    this.data.add(p);

    // On met à jour les sommes
    sommeX += p.getX();
    sommeY += p.getY();

    // On recalcule l'interpolation
    computeInterpolation();
    computeCorrelCoeff();
  }
  
  /**
   * Réalise tout le calcul de l'interpolation
   */
  private void performComputation() {
    
	// Calcul de la somme des X et Y
    if(data.size() > 0) {
      
      sommeX = 0.0;
      sommeY = 0.0;
      
      for(Point p : data) {
        sommeX += p.getX();
        sommeY += p.getY();
      }
      
      // On calcule l'interpolation
      computeInterpolation();
      computeCorrelCoeff();
    }
  }
  
  /**
   * Retourne la valeur de beta0 pour la droite d'interpolation
   * d'équation y = beta1 * x + beta0
   * @return la valeur de beta0
   */
  public double getBeta0() {
    return beta0;      
  }

  /**
   * Retourne la valeur de beta1 pour la droite d'interpolation
   * d'équation y = beta1 * x + beta0
   * @return la valeur de beta1
   */
  public double getBeta1() {
    return beta1;
  }
  
  /**
   * Revoie la valeur absolue du coefficient d'interpolation r,
   * indicateur de la bonne interpolation des points par la droite.
   * @return  |r|
   */
  public double getCorrelCoeffAbs() {
    return Math.abs(correlCoeff);
  }
  
  /**
   * Calcule les valeurs de beta0 et beta1
   */
  private void computeInterpolation() {
    xxBarre = 0.0;
    xyBarre = 0.0;
    yyBarre = 0.0;
    
    xBarre = sommeX / data.size();
    yBarre = sommeY / data.size();
    
    for(Point p : data) {
      xxBarre += (p.getX() - xBarre) * (p.getX() - xBarre);
      yyBarre += (p.getY() - yBarre) * (p.getY() - yBarre);
      xyBarre += (p.getX() - xBarre) * (p.getY() - yBarre);
    }
    
    if(xxBarre != 0) {
      beta1 = xyBarre / xxBarre;
    }
    beta0 = yBarre - beta1 * xBarre;
    
  }
  
  /**
   * Calcule la valeur de r 
   */
  private void computeCorrelCoeff() {
    double sommeMoindresCarrees = 0.0;
    
    for(Point p : data) {
      double yCalc = p.getX() * beta1 + beta0;
      sommeMoindresCarrees += (yCalc - yBarre) * (yCalc - yBarre);
    }
    
    if(yyBarre != 0) {
      correlCoeff = Math.sqrt(sommeMoindresCarrees / yyBarre);
    } else {
      correlCoeff = 0.0;
    }
    
  }

}
