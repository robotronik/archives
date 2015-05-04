/* /!\ l'épaisseur du plan incliné n'est pas exactement celle de epaisseur_base.*/

cote1=22;
cote2=32;
epaisseur_triangles=4;
largeur=40;
epaisseur_base=4;

module triangle(c1,c2,ep)
{
linear_extrude(height=ep)
	polygon(points=[[0,0],[c1,0],[0,c2]]);
}

//translate([0,0,100*$t])
union()
{
difference()
{
	triangle(cote1,cote2,largeur);
	translate([0,epaisseur_base,epaisseur_triangles])
		triangle(cote1-2*epaisseur_base,cote2-2*epaisseur_base,largeur-2*epaisseur_triangles);
}
translate([0,0,(largeur-3)/2])
	linear_extrude(height=3)
		polygon(points=[[0,0],[cote1,0],[5,cote2-10]]);
}