with Ada.Integer_Text_IO, Ada.Text_IO;
use Ada.Integer_Text_IO, Ada.Text_IO;

generic
	type Element is private;
	with procedure Put(E : in Element);
	with function "<" (A, B : Element) return Boolean;
	
Package List is

	Erreur_Liste_Vide: Exception;
	
	type Cellule;
	type Liste is Access Cellule;
	type Cellule is record
		Val: Element;
		Suiv: Liste;
	end record;
	
	function Nouvelle_Liste return Liste;
	
	function Duplique_Liste (L: Liste) return Liste;
	
	procedure Liberer_Cellule (L: in out Liste);
	
	procedure Liberer_Liste (L: in out Liste);
	
	function Longueur_Liste (L: in Liste) return Natural;
	
	function Est_Vide (L: in Liste) return Boolean;
	
	procedure Ajout_tete (L: in out Liste; E: in Element);
	
	procedure Ajout_queue (L: in out Liste; E: in Element);
	
	procedure Afficher (L: in Liste);
	
	procedure Inverse(L: in out Liste);
	
	-- Insertion en position d'une cellule existante, dans une liste L TRIEE.
	procedure Insere_En_Place (A_Inserer: in Liste; L: in out Liste);
	
	procedure Tri_Insertion(L : in out Liste);

end List;
