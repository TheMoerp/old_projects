import java.util.ArrayList;

public class Zeichenblatt {
	public ArrayList<Form> formen;
	
	public Zeichenblatt() {
		this.formen = new ArrayList<Form>();
	}
	
	public void formHinzufuegen(Form form) {
		this.formen.add(form);
	}
	
	public double alleFlaechenBerechnenUndAufsummieren() {
		double summeFlaechen = 0.0;
			
		for (Form f : formen) {
			summeFlaechen += f.flaecheBerechnen();
		}
		
		return summeFlaechen;
	}
}
