#include "../../sources/AlphaDelta.h"
#include "../../sources/QEI.h"
#include "../../sources/XYAngle.h"
#include "../../sources/asser.h"
#include "../../sources/motors.h"
#include "Vars.h"

var_t *getVars() {
	static var_t vars[] = {
		{&distL, sizeof(distL), "distL", TRUE},
		{&distR, sizeof(distR), "distR", TRUE},
		{&vitL, sizeof(vitL), "vitL", TRUE},
		{&vitR, sizeof(vitR), "vitR", TRUE},
		{&delta, sizeof(delta), "delta", TRUE},
		{&alpha, sizeof(alpha), "alpha", TRUE},
		{&vitDelta, sizeof(vitDelta), "vitDelta", TRUE},
		{&vitAlpha, sizeof(vitAlpha), "vitAlpha", TRUE},
		{&posX, sizeof(posX), "posX", FALSE},
		{&posY, sizeof(posY), "posY", FALSE},
		{&consigneX, sizeof(consigneX), "consigneX", FALSE},
		{&consigneY, sizeof(consigneY), "consigneY", FALSE},
		{&erreurX, sizeof(erreurX), "erreurX", TRUE},
		{&erreurY, sizeof(erreurY), "erreurY", TRUE},
		{&posAlpha, sizeof(posAlpha), "posAlpha", FALSE},
		{&consAlpha, sizeof(consAlpha), "consAlpha", FALSE},
		{&erreurAlpha, sizeof(erreurAlpha), "erreurAlpha", FALSE},
		{&aFreinage, sizeof(aFreinage), "aFreinage", FALSE},
		{&alphaCibleDeplac, sizeof(alphaCibleDeplac), "alphaCibleDeplac", FALSE},
		{&erreurAlphaDeplac, sizeof(erreurAlphaDeplac), "erreurAlphaDeplac", FALSE},
		{&erreurAlphaDeplacMod, sizeof(erreurAlphaDeplacMod), "erreurAlphaDeplacMod", FALSE},
		{&erreurCarre, sizeof(erreurCarre), "erreurCarre"},
		{&dFreinageCarre, sizeof(dFreinageCarre), "dFreinageCarre"},
		{&consMDelta, sizeof(consMDelta), "consMDelta", TRUE},
		{&consMAlpha, sizeof(consMAlpha), "consMAlpha", TRUE},
		{&commandeDeltaPrec, sizeof(commandeDeltaPrec), "commandeDeltaPrec", TRUE},
		{&commandeAlphaPrec, sizeof(commandeAlphaPrec), "commandeAlphaPrec", TRUE},
		{&commandeL, sizeof(commandeL), "commandeL", TRUE},
		{&commandeR, sizeof(commandeR), "commandeR", TRUE},
		{&vitLMoy, sizeof(vitLMoy), "vitLMoy", TRUE},
		{&vitLMoyHaut, sizeof(vitLMoyHaut), "vitLMoyHaut", TRUE},
		{&commandeLHaut, sizeof(commandeLHaut), "commandeLHaut", TRUE},
		{&nivBatInv, sizeof(nivBatInv), "nivBatInv", TRUE}
	};	// Mettre à jour NB_VARS dans Vars.h
	return vars;
}
