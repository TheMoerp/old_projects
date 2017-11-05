
public class Rechteck extends Form {
	public double seite1, seite2;
	
	public Rechteck(double seite1, double seite2) {
		this.seite1 = seite1;
		this.seite2 = seite2;
	}
	
	public double flaecheBerechnen() {
		return this.seite1 * this.seite2;
	}

}
