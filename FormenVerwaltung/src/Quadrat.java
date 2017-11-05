
public class Quadrat extends Form {
	public double kantenLaenge;
	
	public Quadrat(double kantenLaenge) {
		this.kantenLaenge = kantenLaenge;
	}
	
	public double flaecheBerechnen() {
		return this.kantenLaenge * this.kantenLaenge;
	}
}
