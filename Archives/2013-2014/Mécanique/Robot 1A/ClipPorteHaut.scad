module forme()
{
difference()
{
translate([0,0,1])cube([20,20,2],center=true);
union()
{
rotate([0,0,120+45]) cube([20,20,2]);
rotate([0,0,-120+45]) cube([20,20,2]);
translate([0,-10,1]) cube([20,20,2],center=true);
}
}
}

difference()
{
forme();
translate([0,1,1]) forme();
}
