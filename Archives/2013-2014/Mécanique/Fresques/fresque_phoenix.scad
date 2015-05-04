include <phoenix.scad>;
include <fresque_basique.scad>;

difference()
{
base();
scale([0.8,0.8,1])
translate([62,50,4])
rotate([0,0,90])
phoenix();
}
