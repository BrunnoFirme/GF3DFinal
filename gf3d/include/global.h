#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "entity.h"

Entity* Player;
Vector3D forward;
Vector3D right;
Entity* EquippedItem;
Sprite* spriteToDraw;
int damage;
int mana;
int maxMana;
int health;
int maxHealth;
Uint8 UI;
Uint8 isShopping;
Uint8 isAttacking;
BoundingBox* PlayerHurtBox;
void Recall();
void MegaJump();
void Heal();
void HealthToMana();
void SummonGold();
Uint8 EnoughMana(int cost);
void LoadConfig(char* filename);
Model* worldModel;
#endif