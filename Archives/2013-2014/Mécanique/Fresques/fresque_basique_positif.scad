$fn=50;
longueur=100;
largeur=80;
epaisseur=3;
rayon_aimant=3;
epaisseur_aimant=2;
decalage=0;
cote_plot=2*rayon_aimant+2;

module plot()
{
difference()
{
cube([cote_plot,cote_plot,epaisseur_aimant]);
translate([cote_plot/2,cote_plot/2,0]) cylinder(h=epaisseur_aimant,r=rayon_aimant);
}
}

module base()
{

	cube([longueur,largeur,epaisseur]);
	translate([0,0,epaisseur])
	union()
{
		translate([decalage,decalage,0])
			plot();
		translate([longueur-decalage-cote_plot,decalage,0])
			plot();
		translate([decalage,largeur-decalage-cote_plot,0])
			plot();
		translate([longueur-decalage-cote_plot,largeur-decalage-cote_plot,0])
			plot();
		//translate([longueur/2,largeur/2,0])
			//cylinder(h=epaisseur_aimant,r=rayon_aimant);
}
}

//base();

