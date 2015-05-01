with Ada.Integer_Text_IO, Ada.Text_IO;
with ADA.Unchecked_Deallocation;
use Ada.Integer_Text_IO, Ada.Text_IO;

package body List is
	procedure Liberer is new ADA.Unchecked_Deallocation(Cellule, Liste);

	function Nouvelle_Liste return Liste is
	begin
		return null;
	end Nouvelle_Liste;
	
	function Duplique_Liste (L: Liste) return Liste is
		Res : Liste := null;
		In_List : Liste := L;
	begin
		while not Est_Vide(In_List) loop
			Ajout_Queue(Res, In_List.Val);
			In_List := In_List.Suiv;
		end loop;
		return Res;
	end Duplique_Liste;
	
	procedure Liberer_Cellule (L: in out Liste) is
	begin
		if not Est_Vide(L) then
			Liberer(L);
			L := null;
		end if;
	end Liberer_Cellule;
	
	procedure Liberer_Liste (L: in out Liste) is
	begin
		if not Est_Vide(L) then
			Liberer_Liste(L.Suiv);
			Liberer(L);
			L := null;
		end if;
	end Liberer_Liste;
	
	function Longueur_Liste (L: in Liste) return Natural is
		Longueur : Natural := 0;
		L_Interne : Liste := L;
	begin
		while not Est_Vide(L_Interne) loop
			Longueur := Longueur + 1;
			L_Interne := L_Interne.Suiv;
		end loop;
		return Longueur;
	end Longueur_Liste;

	function Est_Vide(L: Liste) return Boolean is
	begin
		if L = Null then return True;
		else return False;
		end if;
	end Est_Vide;

	procedure Ajout_tete (L: in out Liste; E: in Element) is
	begin
		L := new Cellule'(E, L);
	end Ajout_tete;
	
	procedure Ajout_queue (L: in out Liste; E: in Element) is
		Tmp: Liste := L;
	begin
		if Est_Vide(L) then
			Ajout_tete(L, E);
		else
			while not Est_Vide(Tmp.Suiv) loop
				Tmp := Tmp.Suiv;
			end loop;
			Tmp.Suiv := new Cellule'(E, null);
		end if;
	end Ajout_queue;
	
	procedure Afficher (L: in Liste) is
		Tmp: Liste;
	begin
		if Est_Vide(L) then
			Put_Line("Liste vide");
		else
			Put_Line("Liste :");
			Tmp := L;
			while not Est_Vide(Tmp) loop
				Put(Tmp.Val);
				Put_Line("");
				Tmp := Tmp.all.Suiv;
			end loop;
		end if;
	end Afficher;
	
	procedure Inverse(L: in out Liste) is
		Cour, Tmp: Liste;
	begin
		if not Est_vide(L) then
			Cour := L;
			while Cour.suiv /= null loop
				Tmp := L;
				L := Cour.suiv;
				Cour.suiv := Cour.suiv.suiv;
				Cour.suiv.suiv := Tmp;
				Cour := Cour.suiv;
			end loop;
		end if;
	end Inverse;
	
	-- Insertion en position d'une cellule existante, dans une liste L TRIEE.
	procedure Insere_En_Place (A_Inserer: in Liste; L: in out Liste) is
	begin
		if L = null or else A_Inserer.Val < L.Val then
			-- insertion en tete
			A_Inserer.Suiv := L;
			L := A_Inserer;
		else
			Insere_En_Place(A_Inserer, L.Suiv);
		end if;
	end Insere_En_Place;


	-- Tri par insertion
	procedure Tri_Insertion(L : in out Liste) is
		A_Inserer, L_Triee : Liste;
	begin
		while L /= null loop
			A_Inserer := L;
			L := L.Suiv;
			Insere_En_Place(A_Inserer, L_Triee);
		end loop;
		L := L_Triee;
	end Tri_Insertion;
end List;
