
CFLAGS = # -g  # A decommenter si necessaire

SRC_PACKAGES = dico.ads dico.adb \
               code.ads code.adb \
               file_priorite.ads file_priorite.adb \
               arbre_huffman.ads arbre_huffman.adb

EXE = tp_huffman


all: $(EXE)

tp_huffman: tp_huffman.adb $(SRC_PACKAGES)
	gnatmake $(CFLAGS) $@


clean:
	gnatclean -c *
	rm -f b~* ~*

cleanall: clean
	rm -f $(EXE)

