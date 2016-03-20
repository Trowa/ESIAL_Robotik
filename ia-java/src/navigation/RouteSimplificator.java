package navigation;

import java.security.InvalidParameterException;
import java.util.ArrayList;

/**
 * Une classe pour simplifier une trajectoire avec trop de points :
 * recherche des segments de droite dans la trajectoire, et retourne
 * uniquement les points extrémités de ces segments.
 * @author Jean-Baptiste HERVÉ
 */
public class RouteSimplificator {
  
  /**
   * Erreur maximale de trajectoire
   */
  private double errorMax;
  
  /**
   * Instancie un nouveau simplificateur de route
   * @param errorMax Erreur maximale acceptable, en mm
   */
  public RouteSimplificator(int errorMax) {
    this.setErrorMax(errorMax);
  }
  
  /**
   * Parametrage de l'erreur maximale acceptable
   * @param errorMax Erreur maximale acceptable, en mm
   */
  public void setErrorMax(int errorMax) {
    if(errorMax > 0) {
      this.errorMax = (double) errorMax;
    } else {
      throw new InvalidParameterException("errorMax doit être > 0");
    }
  }
  
  /**
   * Calcule une route simplifiée à partir de la liste des points à parcourir
   * passée en paramètre.
   * La trajectoire calculée ne s'éloignera pas de {@link #errorMax} mm de la trajectoire
   * initiale.
   * 
   * @param complicatedRoute
   * @return
   */
  public ArrayList<Point> getSimplifiedRoute(ArrayList<Point> complicatedRoute) {
    
    ArrayList<Point> simpleRoute = new ArrayList<Point>(); // Sortie : route simplifiée
    ArrayList<Point> ligneDroiteCourante = new ArrayList<Point>(); // Ligne droite interpolée actuelle
    RegressionLineaire regression = new RegressionLineaire(); // Interpolateur par régression linéaire
    int dernierPointCorrectIndex = 0; // Index du dernier point qui a amélioré l'interpolation
    double dernierCoeffInterp = 0.0; // Dernier coeff d'interpolation
    
    // Si on a des points
    if(complicatedRoute.size() > 0) {
      
      // On prend le premier point, on l'ajoute à la route de sortie et à la ligne courante
      simpleRoute.add(complicatedRoute.get(0));
      ligneDroiteCourante.add(complicatedRoute.get(0));
      
      // On initialise l'interpolateur avec la droite courante (un seul point pour le moment)
      regression.setData(ligneDroiteCourante);
      
      // Pour chaque nouveau point
      int i = 1;
      while(i < (complicatedRoute.size() - 1)) {
        
        // On récupère le point courant
        Point currentPoint = complicatedRoute.get(i);
        
        // On l'ajoute à la ligne courante, on calcule la régression linéaire
        ligneDroiteCourante.add(currentPoint);
        regression.addPoint(currentPoint);
        
        // On teste la qualité de l'interpolation
        if(regression.getCorrelCoeffAbs() >= dernierCoeffInterp) {
          // Le point ajouté a amélioré l'interpolation : il est bien aligné sur la droite
          dernierPointCorrectIndex = i;
          dernierCoeffInterp = regression.getCorrelCoeffAbs();
          
        } else {
          /* 
           * Le dernier point dévie de l'interpolation ; calcul de la distance
           * entre le point courant et la droite d'interpolation
           */
          double distance = Math.abs((regression.getBeta1() * currentPoint.getX())
                                  - currentPoint.getY() + regression.getBeta0())
                            / Math.sqrt(1 + (regression.getBeta1() * regression.getBeta1()));
          
          
          if(distance > errorMax) {
            /* 
             * Le point est trop loin de l'interpolation. On arrête la ligne au dernier
             * point qui avait amélioré l'interpolation
             */
            
            // On reprend le calcul à partir de ce point
            i = dernierPointCorrectIndex;
            
            // On l'ajoute à la route simple
            simpleRoute.add(complicatedRoute.get(dernierPointCorrectIndex));
            
            // On réinitialise la ligne courante
            ligneDroiteCourante.clear();
            ligneDroiteCourante.add(complicatedRoute.get(dernierPointCorrectIndex));
            
            // On reprend le calcul d'interpolation
            regression.setData(ligneDroiteCourante);
            dernierCoeffInterp = 0.0;
            
          } else {
            // Le dernier point n'est pas loin de l'interpolation, on continue
            dernierCoeffInterp = regression.getCorrelCoeffAbs();
          }
        }
        
        // On passe au point suivant
        i++;
      }

      // On rajoute le dernier point de la route
      simpleRoute.add(complicatedRoute.get(complicatedRoute.size() - 1));
      
    }
    return simpleRoute;
  }

}
