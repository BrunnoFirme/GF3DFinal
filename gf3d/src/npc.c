#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"
#include "npc.h"
#include "global.h"
#include "time.h"

Entity* npc_new(Vector3D position, float width, float depth, float health, char* name, Matrix4 modelMatrix, Model* _model)
{
	Entity* ent = NULL;
	ent = gf3d_entity_new();
	if (!ent)
	{
		slog("Cant make Enemy");
		return NULL;
	}

	ent->speed = 1;
	ent->think = npc_think;
	ent->position = position;
	ent->boudingBox = boundingBox(vector2d(position.x, position.y), width, depth);
	ent->health = health;
	ent->name = name;
	ent->model = _model;
	ent->modelMat = modelMatrix;
	return ent;
}

void npc_think(Entity* self)
{
	if (!Player || !self || !PlayerHurtBox)
		return;
	self->boudingBox->position = vector2d(self->position.x - (self->boudingBox->width / 2), self->position.y - (self->boudingBox->height / 2));

	gfc_matrix_make_translation(
		self->modelMat,
		vector3d(self->position.x, self->position.y, self->position.z)
	);

	if (isAttacking == 1 && IsColliding(PlayerHurtBox, self->boudingBox) || self->_inRange == 1 && IsColliding(PlayerHurtBox, self->boudingBox))
	{
		UI = 1;
		self->_inRange = 1;
		if (self->sprite)
		{
			spriteToDraw = self->sprite;
		}
		if (self->_isShop == 1 && isShopping == 0)
		{
			isShopping = 1;
			slog("Player has %i gold", Player->gold);
		}
	}

	if (!IsColliding(PlayerHurtBox, self->boudingBox))
	{
		if (self->_inRange == 1)
		{
			spriteToDraw = NULL;
			UI = 0;
			isShopping = 0;
		}
		self->_inRange = 0;
	}
}
