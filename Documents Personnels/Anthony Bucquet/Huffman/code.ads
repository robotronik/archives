with List;

-- Representation d'un code binaire, suite de bits 0 ou 1.
-- D'autres operations peuvent etre ajoutees si necessaire...

package code is

	type Bit is new Natural range 0 .. 1;
	ZERO : constant Bit := 0;
	UN : constant Bit := 1;

	Code_Vide, Code_Trop_Court : exception;

	type Code_Binaire is private;

	function Cree_Code return Code_Binaire;
	function Cree_Code(C : in Code_Binaire) return Code_Binaire;

	procedure Libere_Code(C : in out Code_Binaire);
	
	function Longueur(C : in Code_Binaire) return Natural;

	procedure Affiche(C : in Code_Binaire);

	procedure Ajoute_Avant(B : in Bit; C : in out Code_Binaire);
	procedure Ajoute_Apres(B : in Bit; C : in out Code_Binaire);
	-- ajoute les bits de C1 apres ceux de C
	procedure Ajoute_Apres(C1 : in Code_Binaire; C : in out Code_Binaire);
	
	function Existe (C : Code_Binaire) return Boolean;


------------------------------------------------------------------------
--   PARCOURS D'UN CODE VIA UN "ITERATEUR"
--   Permet un parcours sequentiel du premier au dernier bit d'un code
--
--   Ex d'utilisation:
--     Parcours_Init(C);
--     while Parcours_Has_Next(C) loop
--         B := Parcours_Next(C);
--         ...
--     end loop;
------------------------------------------------------------------------

	Code_Entierement_Parcouru : exception;

	procedure Parcours_Init(C : Code_Binaire);
	function Parcours_Has_Next(C : Code_Binaire) return Boolean;
	function Parcours_Next(C : Code_Binaire) return Bit;

private

	-- type prive: a definir dans le body du package, code.adb
	type Code_Binaire_Interne;
	type Code_Binaire is access Code_Binaire_Interne;

end code;
