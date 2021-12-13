#ifndef __NPC_H__
#define __NPC_H__
#include "entity.h"

Entity* npc_new(Vector3D position, float width, float depth, float health, char* name, Matrix4 modelMatrix, Model* _model);
void npc_think(Entity* self);


#endif