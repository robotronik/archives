with code; use code;
with Arbre_Huffman; use Arbre_Huffman;
with Ada.Streams.Stream_IO; 

-- Dictionnaire dont les cles sont des caracteres.
-- Les valeurs associees a un caractere sont:
--   1. son nombre d'occurences
--   2. son code binaire

package Dico is
	
	type Dico_Caracteres is private;
	
	Caractere_Absent, Codes_Non_Generes : exception;

	-- Libere le dictionnaire D
	-- garantit: en sortie toute la memoire a ete libere, et D = null.
	procedure Libere_Dico(D : in out Dico_Caracteres);
	
	-- Affiche pour chaque caractere: son nombre d'occurence et son code
	-- (s'il a ete genere)
	procedure Affiche(D : in Dico_Caracteres);
	
	-- Lit un fichier texte, et cree le dictionnaire des caracteres 
	-- presents dans le fichier. Seul le nombre d'occurences est
	-- renseigne.
	procedure Lit_Nb_Occurences(Nom_Fichier : in String;
	                            D : out Dico_Caracteres);

	-- Lit le contenu du dictionnaire dans le flot passe en parametre.
	-- Seuls les caracteres et leur nombre d'occurences sont lus.
	procedure Lit_Dico(D : out Dico_Caracteres;
	          Fichier : Ada.Streams.Stream_IO.File_Type);

	-- Stocke le contenu du dictionnaire dans le flot passe en parametre.
	-- Seuls les caracteres et leur nombre d'occurences sont sauvegardes.
	procedure Ecrit_Dico(D : in Dico_Caracteres;
	          Fichier : Ada.Streams.Stream_IO.File_Type);
	          
	          
	-- Genere les codes binaires des caracteres d'un dictionnaire.
	-- Cette etape necessite de construire un arbre de Huffmann,
	-- puis de le parcourir. Les codes de chaque caractere sont
	-- ensuite stockes dans le dictionnaire.
	procedure Genere_Codes(D : in out Dico_Caracteres);
	
	-- Retourne l'arbre de Huffman correspondant au dictionnaire
	--  leve l'exception Codes_Non_Generes si Genere_Codes 
	--  n'a pas ete appelee.
	function Get_Arbre_Huffman(D : Dico_Caracteres) return Arbre;
	

	-- Retourne le code binaire d'un caractere
	--  -> leve l'exception Codes_Non_Generes si Genere_Codes 
	--  n'a pas ete appelee.
	--  -> leve l'exception Caractere_Absent si C n'est pas dans D
	function Get_Code(C : Character; D : Dico_Caracteres)
	         return Code_Binaire;

	-- Retourne le nombre de caracteres differents dans D
	function Nb_Cles(D : in Dico_Caracteres) return Natural;

	-- Retourne le nombre total de caracteres
	--  =  somme des nombre d'occurences de tous les caracteres de D
	function Nb_Total_Caracteres(D : in Dico_Caracteres) return Natural;

private 
	-- Le type Dico_Caracteres_Interne doit etre defini dans le corps
	-- du package (dico.adb)
	type Dico_Caracteres_Interne;

	type Dico_Caracteres is access Dico_Caracteres_Interne;
	
end Dico;
