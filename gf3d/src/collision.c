#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "collision.h"

Vector2D GetPoint(int isTop, int isRight, BoundingBox *boundingBox)
{
	if (!boundingBox) return;
	Vector2D returnPoint = vector2d(0,0);

	returnPoint.y = (isTop*boundingBox->height) + ((isTop-1)*boundingBox->height);
	returnPoint.y += boundingBox->position.y;

	returnPoint.x = (isRight * boundingBox->width) + ((isRight - 1) * boundingBox->width);
	returnPoint.x += boundingBox->position.x;

	return returnPoint;
}

BoundingBox *boundingBox(Vector2D position, float width, float depth)
{
	BoundingBox *box = NULL;
	box = gfc_allocate_array(sizeof(BoundingBox), 1);
	if (box == NULL)
	{
		slog("failed to allocate data for Bounding Box");
		return NULL;
	}
	box->position = position;
	box->width = width;
	box->height = depth;

	return box;
}

BoundingBox* boundingBoxPos(Vector2D startPosition, Vector2D endPosition)
{
	BoundingBox* box = NULL;
	box = gfc_allocate_array(sizeof(BoundingBox), 1);
	if (box == NULL)
	{
		slog("failed to allocate data for Bounding Box");
		return NULL;
	}
	slog("%f", startPosition.x);
	slog("%f", startPosition.y);
	slog("%f", endPosition.x);
	slog("%f", endPosition.y);
	slog("%f", box->position.y);
	box->position.x = startPosition.x;
	box->position.y = startPosition.y;
	slog("%f", box->position.y);
	box->width = startPosition.x - endPosition.x;
	if (box->width < 0)
		box->width *= -1;
	slog("%f", box->width);
	box->height = startPosition.y - endPosition.y;
	if (box->height < 0)
		box->height *= -1;
	slog("%f", box->height);
	return box;
}

void boundingBoxFree(BoundingBox *boundingBox)
{
	if (!boundingBox) return;
	free(boundingBox);
}

Uint8 IsColliding(BoundingBox *first, BoundingBox *second)
{
	if (!first || !second)
	{
		slog("ERROR null bounding box");
		return NULL;
	}

	if (first->position.x + first->width < second->position.x || first->position.x > second->position.x + second->width ||
		first->position.y + first->height < second->position.y || first->position.y > second->position.y + second->height)
	{
		return 0;
	}

	return 1;
}