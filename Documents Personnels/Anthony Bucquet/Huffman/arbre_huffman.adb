with Ada.Integer_Text_IO, Ada.Text_IO;
with Ada.Unchecked_Deallocation;
use Ada.Integer_Text_IO, Ada.Text_IO;

-- paquetage representant un arbre de Huffman de caracteres

package body Arbre_Huffman is
	procedure Libere is new Ada.Unchecked_Deallocation (Noeud, Arbre);

	-- Cree une feuille a partir d'un caractere
	function Cree_Feuille(C : in Character) return Arbre is
		Feuille : Arbre;
	begin
		Feuille := new Noeud;
		Feuille.C := C;
		return Feuille;
	end Cree_Feuille;

	-- Cree et retourne l'arbre A ayant A1 pour fils gauche et A2
	-- pour fils droit.
	-- Le caractere porte par la racine A n'a pas de signification.
	function Cree_Arbre(A1, A2 : in Arbre) return Arbre is
		A : Arbre;
	begin
		A := new Noeud;
		A.Fg := A1;
		A.Fd := A2;
		return A;
	end Cree_Arbre;

	-- Libere l'arbre de racine A.
	-- garantit: en sortie toute la memoire a ete libere, et A = null.
	procedure Libere_Arbre(A : in out Arbre) is
	begin
		if not Est_Feuille(A) then
			Libere_Arbre(A.Fg);
			Libere_Arbre(A.Fd);
		end if;
		Libere(A);
		A := null;
	end Libere_Arbre;

	-- Affiche les caractères présent dans l'arbre de Huffman
	-- Part de la racine, affiche les déplacements vers le bas de l'arbre
	-- Saute une ligne au retour vers la racine
	procedure Affiche(A : in Arbre) is
	begin
		if A = null then
			Put_Line("Arbre vide.");
		elsif Est_Feuille(A) then
			Put(A.C);
			Put_Line("");
		else
			Put("Fils gauche : ");
			Affiche(A.Fg);
			Put("Fils droit : ");
			Affiche(A.Fd);
		end if;
	end Affiche;
	

-- PROCEDURES ET ACCESSEURS (pour le parcours d'un arbre)

	-- requiert: A /= Arbre_Vide
	function Est_Feuille(A : in Arbre) return Boolean is
	begin
		return (A.Fg = null) and (A.Fd = null);
	end Est_Feuille;

	-- requiert: A /= Arbre_Vide et not Est_Feuille(A)
	function Fils_Gauche(A : in Arbre) return Arbre is
	begin
		return A.Fg;
	end Fils_Gauche;

	-- requiert: A /= Arbre_Vide et not Est_Feuille(A)
	function Fils_Droit(A : in Arbre) return Arbre is
	begin
		return A.Fd;
	end Fils_Droit;
	
	-- Retourne le caractere porte par l'arc
	-- requiert: A /= Arbre_Vide et Est_Feuille(A)
	--  leve l'exception Caractere_Invalide sinon
	function Caractere(A : in Arbre) return Character is
	begin
		if not A.C'Valid then
			raise Caractere_Invalide;
		end if;
		return A.C;
	end Caractere;

end Arbre_Huffman;

