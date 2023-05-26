#ifndef SAVE
#define SAVE

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"

void DeleteSave();
void SaveGame(Game game);
int LoadSave(Game* game);

#endif
 