module pointe()
{
difference()
{
cube([30,25,4.5]);
translate([15,17,0])
rotate([0,0,0])
union()
{
rotate([0,0,-30]) translate([-20,0,0]) cube([40,25,4.5]);
rotate([0,0,30])  translate([-20,0,0]) cube([40,25,4.5]);
}
}
}

difference()
{
pointe();
union()
{
translate([0,-4,1.5]) pointe();
translate([0,-2,3.5]) pointe();
}
}