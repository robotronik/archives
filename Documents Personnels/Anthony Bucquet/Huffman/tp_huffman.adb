with Ada.Text_Io; use Ada.Text_Io;
with Ada.Command_Line; use Ada.Command_Line;
with Ada.Streams.Stream_IO; use Ada.Streams.Stream_IO;
with Arbre_Huffman, code, Dico; use Arbre_Huffman, code, Dico;

procedure tp_huffman is

	type Octet is new Integer range 0 .. 255;
	for Octet'Size use 8; -- permet d'utiliser Octet'Input et Octet'Output,
	                      -- pour lire/ecrire un octet dans un flux
	
------------------------------------------------------------------------------
-- COMPRESSION
------------------------------------------------------------------------------

	procedure Compresse(Nom_Fichier_In, Nom_Fichier_Out : in String) is
		FichierIn, FichierOut : Ada.Streams.Stream_IO.File_Type;
		Dico : Dico_Caracteres;
		FluxIn, FluxOut : Ada.Streams.Stream_IO.Stream_Access;
		Bit_Num : Natural := 7;
		Oct : Octet := 0;
		Code : Code_Binaire;
	begin
		-- Ecrit les nombres d'occurences dans le dictionnaire
		Lit_Nb_Occurences(Nom_Fichier_In, Dico);
		-- Ecrit les codes dans le dictionnaire
		Genere_Codes(Dico);
		-- Crée le fichier compressé
		Create(FichierOut, Out_File, Nom_Fichier_Out);
		-- Y ecrit le dictionnaire
		Ecrit_Dico(Dico, FichierOut);
		-- Ouvre son flux pour le completer
		FluxOut := Stream(FichierOut);
		-- Ouvre le flux du fichier à compresser pour le lire
		Open(FichierIn, In_File, Nom_Fichier_In);
		FluxIn := Stream(FichierIn);
			
		-- lecture tant qu'il reste des caracteres
		-- Ecrit les codes dans le fichier compressé
		while not End_Of_File(FichierIn) loop
			Code := Get_Code(Character'Input(FluxIn), Dico);
			Parcours_Init(Code);
			while Parcours_Has_Next(Code) loop
				Oct := Oct + Octet(Parcours_Next(Code) * 2 ** Bit_Num);
				if Bit_Num = 0 then
					Bit_Num := 7;
					Octet'Output(FluxOut, Oct);
					Oct := 0;
				else
					Bit_Num := Bit_Num - 1;
				end if;
			end loop;
		end loop;
		-- Ecrit le dernier octet si il est incomplet
		if Bit_Num /= 7 then
			Octet'Output(FluxOut, Oct);
		end if;
		
		Close(FichierIn);
		Close(FichierOut);
		Libere_Dico(Dico);
		return;
	end Compresse;



------------------------------------------------------------------------------
-- DECOMPRESSION
------------------------------------------------------------------------------

	procedure Decompresse(Nom_Fichier_In, Nom_Fichier_Out : in String) is
		FichierIn, FichierOut : Ada.Streams.Stream_IO.File_Type;
		Dico : Dico_Caracteres;
		FluxIn, FluxOut : Ada.Streams.Stream_IO.Stream_Access;
		Bit_Num : Natural := 0;
		Oct : Octet;
		A, Noeud : Arbre;
		Nb_Caracteres : Natural;
	begin
		-- Ouvre le fichier à décompresser
		Open(FichierIn, In_File, Nom_Fichier_In);
		-- Importe les nombres d'occurences dans le dictionnaire
		Lit_Dico(Dico, FichierIn);
		-- Réouvre le flux du fichier à décompresser pour le lire
		FluxIn := Stream(FichierIn);
		-- Construit l'arbre permettant de convertir les codes en caractères
		A := Get_Arbre_Huffman(Dico);
		-- Initialise la variable utilisée pour se déplacer dans l'arbre
		Noeud := A;
		-- Crée le fichier décompressé
		Create(FichierOut, Out_File, Nom_Fichier_Out);
		-- Ouvre son flux pour le remplir
		FluxOut := Stream(FichierOut);
			
		-- lecture tant qu'il reste des caracteres à écrire
		Nb_Caracteres := Nb_Total_Caracteres(Dico);
		for Num_Char in 1 .. Nb_Caracteres loop
			-- Cherche le caractère dans l'arbre
			while not Est_Feuille(Noeud) loop
				if Bit_Num = 0 then
					-- Lit l'octet suivant du fichier compressé
					Oct := Octet'Input(FluxIn);
					Bit_Num := 7;
				else
					Bit_Num := Bit_Num - 1;
				end if;
				-- Avance dans l'arbre
				if (Oct / 2 ** Bit_Num) mod 2 = 1 then
					Noeud := Fils_Droit(Noeud);
				else
					Noeud := Fils_Gauche(Noeud);
				end if;
			end loop;
			-- Ecrit le caractère trouvé
			Character'Output(FluxOut, Caractere(Noeud));
			-- Réinitialise la variable utilisée pour se déplacer dans l'arbre
			Noeud := A;
		end loop;
		
		Close(FichierOut);
		Libere_Arbre(A);
		Libere_Dico(Dico);
		Close(FichierIn);
		return;
	end Decompresse;


------------------------------------------------------------------------------
-- PG PRINCIPAL
------------------------------------------------------------------------------

begin

	if (Argument_Count /= 3) then
		Put_Line("utilisation:");
		Put_Line("  compression : ./huffman -c fichier.txt fichier.comp");
		Put_Line("  decompression : ./huffman -d fichier.comp fichier.comp.txt");
		Set_Exit_Status(Failure);
		return;
	end if;

	if (Argument(1) = "-c") then
		Compresse(Argument(2), Argument(3));
	else
		Decompresse(Argument(2), Argument(3));
	end if;

	Set_Exit_Status(Success);

end tp_huffman;

