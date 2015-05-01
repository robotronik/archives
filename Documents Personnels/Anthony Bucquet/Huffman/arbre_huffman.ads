-- paquetage representant un arbre de Huffman de caracteres

package Arbre_Huffman is
	
	type Noeud is private;
	type Arbre is Access Noeud;

	Arbre_Vide : constant Arbre := null;

	-- Cree une feuille a partir d'un caractere
	function Cree_Feuille(C : in Character) return Arbre;

	-- Cree et retourne l'arbre A ayant A1 pour fils gauche et A2
	-- pour fils droit.
	-- Le caractere porte par la racine A n'a pas de signification.
	function Cree_Arbre(A1, A2 : in Arbre) return Arbre;

	-- Libere l'arbre de racine A.
	-- garantit: en sortie toute la memoire a ete libere, et A = null.
	procedure Libere_Arbre(A : in out Arbre);

	procedure Affiche(A : in Arbre);
	

-- PROCEDURES ET ACCESSEURS (pour le parcours d'un arbre)

	-- requiert: A /= Arbre_Vide
	function Est_Feuille(A : in Arbre) return Boolean;

	-- requiert: A /= Arbre_Vide et not Est_Feuille(A)
	function Fils_Gauche(A : in Arbre) return Arbre;

	-- requiert: A /= Arbre_Vide et not Est_Feuille(A)
	function Fils_Droit(A : in Arbre) return Arbre;

	Caractere_Invalide : exception;

	-- Retourne le caractere porte par l'arc
	-- requiert: A /= Arbre_Vide et Est_Feuille(A)
	--  leve l'exception Caractere_Invalide sinon
	function Caractere(A : in Arbre) return Character;

private

	type Noeud is Record
		C : Character;
		Fg, Fd : Arbre;
	end Record;

end Arbre_Huffman;

