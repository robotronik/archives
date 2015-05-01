with List, Arbre_Huffman; use Arbre_Huffman;
with Ada.Integer_Text_IO, Ada.Text_IO;
use Ada.Integer_Text_IO, Ada.Text_IO;

-- paquetage generique de file de priorite 
-- Les priorites sont munies d'un ordre total "Est_Prioritaire"

package body File_Priorite is

	type Element is record
		D : Donnee;
		P : Priorite;
	end record;
	
	procedure Put(E : in Element) is
	begin
		Put("Donnee : ");
		PutDonnee(E.D);
		Put(" ; Priorite : ");
		PutPriorite(E.P);
		Put_Line("");
	end Put;
	function "<" (A, B : Element) return Boolean is
	begin
		return Est_Prioritaire(A.P, B.P);
	end "<";
	
	package List_File is new List (Element, Put, "<");
	use List_File;
	type File_Interne is new Liste;

	-- Cree et retourne une nouvelle file, initialement vide
	function Cree_File return File_Prio is
		F : File_Prio := new File_Interne;
	begin
		F.all := Nouvelle_Liste;
		return F;
	end Cree_File;

	-- Libere une file de priorite.
	-- garantit: en sortie toute la memoire a ete libere, et F = null.
	procedure Libere_File(F : in out File_Prio) is
	begin
		Liberer_Liste(F.all);
	end Libere_File;
	
	-- retourne True si la file est vide, False sinon
	function Est_Vide(F: in File_Prio) return Boolean is
	begin
		return Est_Vide(Liste(F.all));
	end Est_Vide;

	-- retourne True si la file est pleine, False sinon
	function Est_Pleine(F: in File_Prio) return Boolean is
	begin
		return False;
	end Est_Pleine;

	-- si not Est_Pleine(F)
	  -- insere la donnee D de priorite P dans la file F
	-- sinon
	  -- leve l'exception File_Pleine
	procedure Insere(F : in out File_Prio; D : in Donnee; P : in Priorite) is
		C : Liste := new Cellule;
	begin
		C.Val.D := D;
		C.Val.P := P;
		Insere_En_Place(C, Liste(F.all));
	end Insere;

	-- si not Est_Vide(F)
	  -- supprime la donnee la plus prioritaire de F.
	  -- sortie: D est la donnee, P sa priorite
	-- sinon
	  -- leve l'exception File_Vide
	procedure Supprime(F: in File_Prio; D: out Donnee; P: out Priorite) is
		Tmp : Liste := Liste(F.all);
	begin
		D := Tmp.Val.D;
		P := Tmp.Val.P;
		F.all := File_Interne(Tmp.Suiv);
		Liberer_Cellule(Tmp);
	end Supprime;

	-- si not Est_Vide(F)
	  -- retourne la donnee la plus prioritaire de F (sans la
	  -- sortir de la file)
	  -- sortie: D est la donnee, P sa priorite
	-- sinon
	  -- leve l'exception File_Vide
	procedure Prochain(F: in File_Prio; D: out Donnee; P: out Priorite) is
		Tmp : Liste := Liste(F.all);
	begin
		D := Tmp.Val.D;
		P := Tmp.Val.P;
	end Prochain;
	
end File_Priorite;

