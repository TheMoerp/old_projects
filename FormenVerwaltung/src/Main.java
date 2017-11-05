
public class Main {

	public static void main(String[] args) {
		try {
			Zeichenblatt z = new Zeichenblatt();

			Quadrat q1 = new Quadrat(2.0);
			z.formHinzufuegen(q1);

			Kreis k1 = new Kreis(1.0);
			k1.setRadius(-2.0);
			z.formHinzufuegen(k1);

			Rechteck r1 = new Rechteck(2.0, 3.0);
			z.formHinzufuegen(r1);

			// Fehler, Form ist abstract
			// Form f1 = new Form();

			System.out.println(z.alleFlaechenBerechnenUndAufsummieren());
		} catch (Exception e) {
			System.out.println(e);
		}
	}

}
