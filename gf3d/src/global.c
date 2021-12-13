#include <SDL.h>            

#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "global.h"

int recallCost = 2;

void Recall()
{
    if (EnoughMana(recallCost))
    {
        Player->position = vector3d(2, 40, 2);
        slog("RECALL");
        mana -= recallCost;
        Player->velocity.z = 0;
    }
}

void MegaJump()
{
    int cost = 3;

    if (EnoughMana(cost))
    {
        Player->velocity = vector3d(Player->velocity.x, Player->velocity.y, Player->velocity.z+500);
        slog("MEGA JUMP");
        mana -= cost;
    }
}

void Heal()
{
    int cost = 1;

    if (EnoughMana(cost))
    {
        health = maxHealth;
        slog("HEAL!");
        mana -= cost;
    }
}

void HealthToMana()
{
    if (health > 0)
    {
        health = 1;
        mana = maxMana;
        slog("BLOOD MAGIC!");
    }
}

void SummonGold()
{
    int cost = 5;

    if (EnoughMana(cost))
    {
        slog("GOLD!");
        mana -= cost;
        Player->gold += 100;
    }
}

Uint8 EnoughMana(int cost)
{
    if (mana >= cost)
        return 1;
    else
        return 0;
}

void LoadConfig(char* filename)
{
    SJson* json, * wjson;
    SJson* recall;
    const char* modelName = NULL;
    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load json file (%s) for the world data", filename);
        return NULL;
    }

    wjson = sj_object_get_value(json, "world");
    recall = sj_object_get_value(json, "Recall");
    SJson* playerJson = sj_object_get_value(json, "Player");

    modelName = sj_get_string_value(sj_object_get_value(wjson, "model"));

    sj_get_integer_value(sj_object_get_value(recall, "cost"), &recallCost);
    sj_get_integer_value(sj_object_get_value(playerJson, "maxHealth"), &maxHealth);
    sj_get_integer_value(sj_object_get_value(playerJson, "mana"), &mana);
    sj_get_integer_value(sj_object_get_value(playerJson, "gold"), &Player->gold);

    if (modelName)
    {
        worldModel = gf3d_model_load(modelName);
    }
    else
    {
        slog("world data (%s) has no model", filename);
    }



    sj_free(json);
}