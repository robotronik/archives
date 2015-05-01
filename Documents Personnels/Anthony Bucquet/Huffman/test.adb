with Ada.Integer_Text_IO, Ada.Text_IO;
with Arbre_Huffman, code, Dico;
use Ada.Integer_Text_IO, Ada.Text_IO;
use Arbre_Huffman, code, Dico;
with Ada.Streams.Stream_IO; use Ada.Streams.Stream_IO;

-- procedure Test is
	-- C1, C2 : Code_Binaire;
-- begin
	-- C1 := Cree_Code;
	-- Ajoute_Avant(0, C1);
	-- Ajoute_Avant(1, C1);
	-- C2 := Cree_Code(C1);
	-- Ajoute_Apres(0, C2);
	-- Ajoute_Apres(C1, C2);
	-- Libere_Code(C1);
	-- Parcours_Init(C2);
	-- while Parcours_Has_Next(C2) loop
		-- Put(Parcours_Next(C2));
	-- end loop;
	-- Libere_Code(C2);
-- end Test;

-- procedure Test is
	-- N1, N2 : Arbre;
-- begin
	-- N1 := Cree_Feuille('A');
	-- N2 := Cree_Feuille('B');
	-- N1 := Cree_Arbre(N1, N2);
	-- N2 := Cree_Feuille('C');
	-- N1 := Cree_Arbre(N1, N2);
	-- Affiche(N1);
	-- Libere_Arbre(N1);
	-- Affiche(N1);
-- end Test;