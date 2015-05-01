with code; use code;
with Arbre_Huffman; use Arbre_Huffman;
with File_Priorite;
with Ada.Integer_Text_IO, Ada.Text_IO; use Ada.Integer_Text_IO, Ada.Text_IO;
with Ada.Streams.Stream_IO; use Ada.Streams.Stream_IO;
with ADA.Unchecked_Deallocation;

with Ada.Characters.Latin_1;

-- Dictionnaire dont les cles sont des caracteres.
-- Les valeurs associees a un caractere sont:
--   1. son nombre d'occurences
--   2. son code binaire

package body Dico is

	function Est_Prioritaire(P1, P2: Natural) return Boolean is
	begin
		return P1 < P2;
	end Est_Prioritaire;
	procedure PutNat(P: Natural) is
	begin
		Put(Integer(P));
	end PutNat;
	
	package Inst_File_Priorite is new File_Priorite (Arbre, Natural, Est_Prioritaire, PutDonnee => Affiche, PutPriorite => PutNat);
	use Inst_File_Priorite;

	type Entree is record
		Caractere : Character;
		Nb_occurence : Natural;
		Code : Code_Binaire;
	end record;
	type Dico_Caracteres_Interne is array (Character) of Entree;

	type Octet is new Integer range 0 .. 255;
	for Octet'Size use 8; -- permet d'utiliser Octet'Input et Octet'Output,
	                      -- pour lire/ecrire un octet dans un flux
	
	procedure Liberer is new ADA.Unchecked_Deallocation(Dico_Caracteres_Interne, Dico_Caracteres);
	
	-- Libere le dictionnaire D
	-- garantit: en sortie toute la memoire a ete libere, et D = null.
	procedure Libere_Dico(D : in out Dico_Caracteres) is
	begin
		for Caractere in Character'range loop
			if Existe(D(Caractere).Code) then
				Libere_Code(D(Caractere).Code);
			end if;
		end loop;
		Liberer(D);
	end Libere_Dico;
	
	-- Affiche pour chaque caractere: son nombre d'occurence et son code
	-- (s'il a ete genere)
	procedure Affiche(D : in Dico_Caracteres) is
	begin
		Put_Line("Dictionnaire :");
		for Caractere in Character'range loop
			if D(Caractere).Nb_occurence > 0 then
				Put(''' & Caractere & "' : ");
				Put(D(Caractere).Nb_occurence);
				if Existe(D(Caractere).Code) then
					Put(' ');
					Affiche(D(Caractere).Code);
				end if;
				Put_Line("");
			end if;
		end loop;
	end Affiche;
	
	-- Lit un fichier texte, et cree le dictionnaire des caracteres 
	-- presents dans le fichier. Seul le nombre d'occurences est
	-- renseigne.
	procedure Lit_Nb_Occurences(Nom_Fichier : in String;
	                            D : out Dico_Caracteres) is
		Fichier : Ada.Streams.Stream_IO.File_Type;
		Flux : Ada.Streams.Stream_IO.Stream_Access;
		C : Character;
	begin
		Open(Fichier, In_File, Nom_Fichier);
		Flux := Stream(Fichier);
		D := new Dico_Caracteres_Interne;
		for Caractere in Character'range loop
			D(Caractere).Nb_occurence := 0;
		end loop;
		
		-- lecture tant qu'il reste des caracteres
		while not End_Of_File(Fichier) loop
			C := Character'Input(Flux);
			D(C).Nb_occurence := D(C).Nb_occurence + 1;
		end loop;
		Close(Fichier);
	end Lit_Nb_Occurences;

	-- Lit le contenu du dictionnaire dans le flot passe en parametre.
	-- Seuls les caracteres et leur nombre d'occurences sont lus.
	procedure Lit_Dico(D : out Dico_Caracteres;
	          Fichier : Ada.Streams.Stream_IO.File_Type) is
		Flux : Ada.Streams.Stream_IO.Stream_Access;
		Char : Character;
		Nb_Cles : Octet;
	begin
		Flux := Stream(Fichier);
		D := new Dico_Caracteres_Interne;
		for Caractere in Character'range loop
			D(Caractere).Nb_occurence := 0;
		end loop;
		
		-- lecture tant qu'il reste des caracteres
		Nb_Cles := Octet'Input(Flux);
		for Cle in 1 .. Nb_Cles loop
			Char := Character'Input(Flux);
			D(Char).Nb_occurence := Natural'Input(Flux);
		end loop;
	end Lit_Dico;

	-- Stocke le contenu du dictionnaire dans le flot passe en parametre.
	-- Seuls les caracteres et leur nombre d'occurences sont sauvegardes.
	procedure Ecrit_Dico(D : in Dico_Caracteres;
	          Fichier : Ada.Streams.Stream_IO.File_Type) is
		Flux : Ada.Streams.Stream_IO.Stream_Access;
	begin
		Flux := Stream(Fichier);

		Octet'Output(Flux, Octet(Nb_Cles(D)));
		for Caractere in Character'range loop
			if D(Caractere).Nb_occurence > 0 then
				Character'Output(Flux, Caractere);
				Natural'Output(Flux, D(Caractere).Nb_occurence);
			end if;
		end loop;
	end Ecrit_Dico;
	
	-- Remplie le dictionnaire avec les codes des caractères de l'arbre
	procedure Parcours_Arbre (A : in Arbre; D : in out Dico_Caracteres; C : in out Code_Binaire) is
		Cg, Cd : Code_Binaire;
	begin
		if Est_Feuille(A) then
			D(Caractere(A)).Code := C;
		else
			Cg := Cree_Code(C);
			Ajoute_Apres(0, Cg);
			Parcours_Arbre (Fils_Gauche(A), D, Cg);
			Cd := Cree_Code(C);
			Ajoute_Apres(1, Cd);
			Parcours_Arbre (Fils_Droit(A), D, Cd);
			Libere_Code(C);
		end if;
	end Parcours_Arbre;
	
	-- Genere les codes binaires des caracteres d'un dictionnaire.
	-- Cette etape necessite de construire un arbre de Huffmann,
	-- puis de le parcourir. Les codes de chaque caractere sont
	-- ensuite stockes dans le dictionnaire.
	procedure Genere_Codes(D : in out Dico_Caracteres) is
		A : Arbre := Get_Arbre_Huffman(D);
		CodeTmp : Code_Binaire := Cree_Code;
	begin
		Parcours_Arbre(A, D, CodeTmp);
		Libere_Arbre(A);
	end Genere_Codes;
	
	-- Retourne l'arbre de Huffman correspondant au dictionnaire
	function Get_Arbre_Huffman(D : Dico_Caracteres) return Arbre is
		F : File_Prio := Cree_File;
		Noeud1, Noeud2 : Arbre;
		Nb_occurence1, Nb_occurence2 : Natural;
	begin
		for Caractere in Character'range loop
			if D(Caractere).Nb_occurence > 0 then
				Insere(F, Cree_Feuille(Caractere), D(Caractere).Nb_occurence);
			end if;
		end loop;
		Supprime(F, Noeud1, Nb_occurence1);
		while not Est_Vide(F) loop
			Supprime(F, Noeud2, Nb_occurence2);
			Noeud1 := Cree_Arbre(Noeud1, Noeud2);
			Insere(F, Noeud1, Nb_occurence1 + Nb_occurence2);
			Supprime(F, Noeud1, Nb_occurence1);
		end loop;
		Libere_File(F);
		return Noeud1;
	end Get_Arbre_Huffman;

	-- Retourne le code binaire d'un caractere
	--  -> leve l'exception Codes_Non_Generes si Genere_Codes 
	--  n'a pas ete appelee.
	--  -> leve l'exception Caractere_Absent si C n'est pas dans D
	function Get_Code(C : Character; D : Dico_Caracteres)
	         return Code_Binaire is
	begin
		if D(C).Nb_occurence = 0 then
			raise Caractere_Absent;
		elsif not Existe(D(C).Code) then
			raise Codes_Non_Generes;
		end if;
		return D(C).Code;
	end Get_Code;

	-- Retourne le nombre de caracteres differents dans D
	function Nb_Cles(D : in Dico_Caracteres) return Natural is
		NbCles : Natural := 0;
	begin
		for Caractere in Character'range loop
			if D(Caractere).Nb_occurence > 0 then
				NbCles := NbCles + 1;
			end if;
		end loop;
		return NbCles;
	end Nb_Cles;

	-- Retourne le nombre total de caracteres
	--  =  somme des nombre d'occurences de tous les caracteres de D
	function Nb_Total_Caracteres(D : in Dico_Caracteres) return Natural is
		NbTotalCaracteres : Natural := 0;
	begin
		for Caractere in Character'range loop
			NbTotalCaracteres := NbTotalCaracteres + D(Caractere).Nb_occurence;
		end loop;
		return NbTotalCaracteres;
	end Nb_Total_Caracteres;
	
end Dico;
