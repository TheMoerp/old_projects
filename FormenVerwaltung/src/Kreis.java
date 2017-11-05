
public class Kreis extends Form {
	private double radius;
	
	public Kreis(double radius) {
		this.radius = radius;
	}
	
	
	public double getRadius() {
		return radius;
	}


	public void setRadius(double radius) throws Exception {
		if (radius <= 0) {
			throw new Exception("Kreis kann keinen negativen oder null Radius haben");
		}
		this.radius = radius;
	}


	public double flaecheBerechnen() {
		return this.radius * this.radius * 3.141;
	}
}
