with List;
with ADA.Unchecked_Deallocation;
with Ada.Integer_Text_IO, Ada.Text_IO;
use Ada.Integer_Text_IO, Ada.Text_IO;

-- Representation d'un code binaire, suite de bits 0 ou 1.
-- D'autres operations peuvent etre ajoutees si necessaire...

package body code is
	
	subtype Element is Bit;
	
	procedure Put(E : in Element) is
	begin
		Put(Integer(Bit(E)));
	end Put;
	function "<" (A, B : Element) return Boolean is
	begin
		return A < B;
	end "<";
	
	package List_File is new List (Element => Element, Put => Put, "<" => "<");
	use List_File;
	type File_Interne is new Liste;

	type Code_Binaire_Interne is record
		Code: Liste;
		Iter: Liste;
	end record;
	
	procedure Liberer is new ADA.Unchecked_Deallocation(Code_Binaire_Interne, Code_Binaire);

	function Cree_Code return Code_Binaire is
		Code : Code_Binaire := new Code_Binaire_Interne;
	begin
		Code.Code := Nouvelle_Liste;
		return Code;
	end Cree_Code;
	-- Duplique un code
	function Cree_Code(C : in Code_Binaire) return Code_Binaire is
		Code : Code_Binaire := new Code_Binaire_Interne;
	begin
		Code.Code := Duplique_Liste(C.Code);
		return Code;
	end Cree_Code;

	procedure Libere_Code(C : in out Code_Binaire) is
	begin
		Liberer_Liste(C.Code);
		Liberer(C);
	end Libere_Code;
	
	function Longueur(C : in Code_Binaire) return Natural is
	begin
		return Longueur_Liste(C.Code);
	end Longueur;

	procedure Affiche(C : in Code_Binaire) is
	begin
		Afficher(C.Code);
	end Affiche;

	procedure Ajoute_Avant(B : in Bit; C : in out Code_Binaire) is
	begin
		Ajout_tete(C.Code, B);
	end Ajoute_Avant;
	procedure Ajoute_Apres(B : in Bit; C : in out Code_Binaire) is
	begin
		Ajout_queue(C.Code, B);
	end Ajoute_Apres;
	-- ajoute les bits de C1 apres ceux de C
	procedure Ajoute_Apres(C1 : in Code_Binaire; C : in out Code_Binaire) is
		IterC: Liste := C.Code;
	begin
		if not Est_Vide(C.Code) then
			-- Va à la fin de C
			while not Est_Vide(IterC.Suiv) loop
				IterC := IterC.Suiv;
			end loop;
			IterC.Suiv := Duplique_Liste(C1.Code);
		else
			IterC := Duplique_Liste(C1.Code);
		end if;
	end Ajoute_Apres;
	
	function Existe (C : Code_Binaire) return Boolean is
	begin
		if C = null then
			return false;
		end if;
		return true;
	end Existe;


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

	procedure Parcours_Init(C : Code_Binaire) is
	begin
		C.Iter := C.Code;
	end Parcours_Init;
	function Parcours_Has_Next(C : Code_Binaire) return Boolean is
	begin
		return not Est_Vide(C.Iter);
	end Parcours_Has_Next;
	function Parcours_Next(C : Code_Binaire) return Bit is
		B : Bit := C.Iter.Val;
	begin
		C.Iter := C.Iter.Suiv;
		return B;
	end Parcours_Next;
end code;
