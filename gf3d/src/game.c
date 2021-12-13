#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gfc_audio.h"
#include "gf3d_sprite.h"

#include "entity.h"
#include "collision.h"
#include "time.h"
#include "global.h"
#include "powerUp.h"
#include "enemy.h"
#include "npc.h"

void player_think(Entity* self)
{
    if (!self)return;
    self->position.z += self->velocity.z * deltaTime;
    if (self->position.z > 0) self->velocity.z += -96.04 *deltaTime;
    if (self->position.z < 0)
    {
        self->position.z = 0;
        self->velocity.z = 0;
    }
}

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    int mouseDraw = 0;
    Uint8 validate = 0;
    Uint8 inMainMenu = 1;
    UI = 0;
    isShopping = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    
    BoundingBox *Forward;
    BoundingBox *Right;
    BoundingBox *Back;
    BoundingBox *Left;
    BoundingBox* SusBB;

    BoundingBox* PlayerBoundingBox;
    maxMana = 5;
    float desiredRot = 0;

    Forward = boundingBox(vector2d(-65, 65), 130, 5);
    Back = boundingBox(vector2d(-65, -65), 130, 5);
    Right = boundingBox(vector2d(65, -65), 5, 130);
    Left = boundingBox(vector2d(-65, -65), 5, 130);
    
    BoundingBox* B1 = boundingBoxPos(vector2d(-15,-13),vector2d(15,-13));
    BoundingBox* B2 = boundingBoxPos(vector2d(-15, -13), vector2d(-15, 265));
    BoundingBox* B3 = boundingBoxPos(vector2d(15, -13), vector2d(15, 265));
    //BoundingBox* B4 = boundingBoxPos(vector2d(-760, 280), vector2d(230, 320));
    BoundingBox* B5 = boundingBoxPos(vector2d(-760, 280), vector2d(-760, 4));
    BoundingBox* B6 = boundingBoxPos(vector2d(-760, 4), vector2d(-568, 0));
    BoundingBox* B7 = boundingBoxPos(vector2d(-568, 0), vector2d(-560, -308));
    BoundingBox* B8 = boundingBoxPos(vector2d(-560, -308), vector2d(-386, -312));
    BoundingBox* B9 = boundingBoxPos(vector2d(-386, -312), vector2d(-391, -234));
    BoundingBox* B10 = boundingBoxPos(vector2d(-391, -234), vector2d(-172, -226));
    BoundingBox* B11 = boundingBoxPos(vector2d(-172, -226), vector2d(-168, -407));
    BoundingBox* B12 = boundingBoxPos(vector2d(-168, -407), vector2d(-114, -407));
    BoundingBox* B13 = boundingBoxPos(vector2d(-114, -407), vector2d(-120, -157));
    BoundingBox* B14 = boundingBoxPos(vector2d(-120, -157), vector2d(-396, -163));
    BoundingBox* B15 = boundingBoxPos(vector2d(-396, -163), vector2d(-401, 263));
    BoundingBox* B16 = boundingBoxPos(vector2d(-401, 263), vector2d(-19, 274));
    //BoundingBox* B17 = boundingBoxPos(vector2d(-19, 274), vector2d(-19, -13));
    //BoundingBox* B18 = boundingBoxPos(vector2d(-15, 265), vector2d(93, 275));
    //BoundingBox* B19 = boundingBoxPos(vector2d(93, 275), vector2d(97, 80));
    BoundingBox* B20 = boundingBoxPos(vector2d(97, 80), vector2d(150, 73));
    BoundingBox* B21 = boundingBoxPos(vector2d(150, 73), vector2d(150, 278));
    BoundingBox* B22 = boundingBoxPos(vector2d(150, 278), vector2d(267, 278));
    BoundingBox* B23 = boundingBoxPos(vector2d(267, 278), vector2d(268, 173));
    BoundingBox* B24 = boundingBoxPos(vector2d(268, 173), vector2d(295, 166));
    BoundingBox* B25 = boundingBoxPos(vector2d(295, 166), vector2d(295, 285));
    BoundingBox* B26 = boundingBoxPos(vector2d(295, 285), vector2d(515, 285));
    BoundingBox* B27 = boundingBoxPos(vector2d(515, 285), vector2d(528, 482));
    BoundingBox* B28 = boundingBoxPos(vector2d(528, 482), vector2d(228, 482));
    BoundingBox* B29 = boundingBoxPos(vector2d(228, 482), vector2d(228, 320));


    PlayerBoundingBox = boundingBox(vector2d(0, 0), 1, 1);
    PlayerHurtBox = boundingBox(vector2d(-2.5f, -1.5f), 5, 10);
    SusBB = boundingBox(vector2d(-20, -30), 40, 50);

    //Forward = 0, Right = 1, Back =2, Left =3
    int currentDirection = 0;
    Sound *susSound;
    Sound* pickUpRing;

    Sprite* mouse = NULL;
    Sprite* MainMenu = NULL;
    Sprite* Mana1 = NULL;
    Sprite* Mana2 = NULL;
    Sprite* Mana3 = NULL;
    Sprite* Mana4 = NULL;
    Sprite* Mana5 = NULL;

    Sprite* TextBox = NULL;
    Sprite* TextBox1 = NULL;
    Sprite* TextBox2 = NULL;

    float frame = 0;
    int mousex, mousey;
    float mouseFrame = 0;
    maxHealth = 100;
    health = maxHealth;
    mana = maxMana;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init
    (
        "The Elder Scrolls V: Skryim",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
    gfc_audio_init
    (
        10,  //Max sounds
        10,  //channels
        10,  //channel groups
        10,  //max music
        1,   //mp3
        1    //ogg
    );
    gf3d_entity_manager_init(2048);
	slog_sync();

    gf3d_camera_set_scale(vector3d(1, 1, 1));

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    susSound = gfc_sound_load("sus.mp3", 10,0);
    pickUpRing = gfc_sound_load("pickUp.wav", 10, 0);
    slog_sync();


    /*
    Model* speedDownModel;
    speedDownModel = gf3d_model_load("SpeedPowerUp");
    Matrix4 speedDownModelMatrix;
    gfc_matrix_identity(speedDownModelMatrix);
    gfc_matrix_make_translation(
        speedDownModelMatrix,
        vector3d(0, -5, 0)
    );
    */
    Model* room;
    room = gf3d_model_load("RoomC");
    Matrix4 RoomModelMatrix;
    gfc_matrix_identity(RoomModelMatrix);
    gfc_matrix_make_translation(
            RoomModelMatrix,
            vector3d(0,0,-15)
    );
    gfc_matrix_rotate(
        RoomModelMatrix,
        RoomModelMatrix,
        130.4,
        vector3d(0, 0, 1)
    );
    
    Player = gf3d_entity_new();
    Player->position = vector3d(2, 40, 2);
    Player->rotation = vector3d(0, 0, 0);
    Player->think = player_think;
    Player->boudingBox = PlayerBoundingBox;
    Player->speed = 1;
    Player->jumpHeight = 1;

    Matrix4 PlayerHandModelMatrix;
    gfc_matrix_identity(PlayerHandModelMatrix);

    float  cameraSpeed = 20;
    float    cooldown = 0;
    

    Model *speedUpModel;
    speedUpModel = gf3d_model_load_M("su", "Colors");
    
    Matrix4 speedUpModelMatrix; gfc_matrix_identity(speedUpModelMatrix);

    Entity* speedUpPowerUp;
    speedUpPowerUp = powerUp_new(vector3d(-88, 294, 1),10,10);
    speedUpPowerUp->model = speedUpModel;
    speedUpPowerUp->rotation = vector3d(0, 0, 0);
    speedUpPowerUp->powerUpFunc = powerUpSpeedAndJump;
    speedUpPowerUp->modelMat = speedUpModelMatrix;
    speedUpPowerUp->speed = 4;
    speedUpPowerUp->sound = pickUpRing;

    Model* speedDownModel;
    speedDownModel = gf3d_model_load_M("sd", "Colors");
    
    Matrix4 speedDownModelMatrix; gfc_matrix_identity(speedDownModelMatrix);
    
    Entity* speedDownPowerUp;
    speedDownPowerUp = powerUp_new(vector3d(-480, -200, 1), 10, 10);
    speedDownPowerUp->model = speedDownModel;
    speedDownPowerUp->rotation = vector3d(0, 0, 0);
    speedDownPowerUp->powerUpFunc = powerDownSpeedAndJump;
    speedDownPowerUp->modelMat = speedDownModelMatrix;
    speedDownPowerUp->speed = 1;
    speedDownPowerUp->sound = pickUpRing;

    Model* goldPickUpModel;
    goldPickUpModel = gf3d_model_load_M("gold", "Colors");
    Matrix4 goldPickUpModelMatrix;
    gfc_matrix_identity(goldPickUpModelMatrix);

    Entity* goldPickUp;
    goldPickUp = powerUp_new(vector3d(-140, -300, 0), 10, 10);
    goldPickUp->model = goldPickUpModel;
    goldPickUp->rotation = vector3d(0, 0, 0);
    goldPickUp->powerUpFunc = powerUpGold;
    goldPickUp->modelMat = goldPickUpModelMatrix;
    goldPickUp->speed = 1;
    goldPickUp->_isItem = 0;
    goldPickUp->sound = pickUpRing;
    

    Model* AxeModel = gf3d_model_load_M("Axe", "Colors");
    Model* ClubModel = gf3d_model_load_M("Club", "Colors");
    Model* Club2Model = gf3d_model_load_M("Club2", "Colors");
    Model* Club3Model = gf3d_model_load_M("Club3", "Colors");
    Model* Club4Model = gf3d_model_load_M("Club4", "Colors");

    Model* weaponModel = AxeModel;
    damage = 1;


    Model* TargetModel = gf3d_model_load_M("Target", "Colors");
    Model* Target2Model = gf3d_model_load_M("Target2", "Colors");
    Model* Target3Model = gf3d_model_load_M("Target3", "Colors");
    Model* Target4Model = gf3d_model_load_M("Target4", "Colors");
    Model* Target5Model = gf3d_model_load_M("Target5", "Colors");

    Matrix4 targetModelMatrix; gfc_matrix_identity(targetModelMatrix);
    Entity* target1 = enemy_new(
        vector3d(-750, 290, -3),
        5,
        5,
        1,
        "Target 1",
        targetModelMatrix,
        TargetModel
    );
    
    Matrix4 target2ModelMatrix; gfc_matrix_identity(target2ModelMatrix);
    Entity* target2 = enemy_new(
        vector3d(-730, 290, -3),
        5,
        5,
        3,
        "Target 2",
        target2ModelMatrix,
        Target2Model
    );

    Matrix4 target3ModelMatrix; gfc_matrix_identity(target3ModelMatrix);
    Entity* target3 = enemy_new(
        vector3d(-710, 290, -3),
        5,
        5,
        5,
        "Target 3",
        target3ModelMatrix,
        Target3Model
    );

    Matrix4 target4ModelMatrix; gfc_matrix_identity(target4ModelMatrix);
    Entity* target4 = enemy_new(
        vector3d(-690, 290, -3),
        5,
        5,
        10,
        "Target 4",
        target4ModelMatrix,
        Target4Model
    );

    Matrix4 target5ModelMatrix; gfc_matrix_identity(target5ModelMatrix);
    Entity* target5 = enemy_new(
        vector3d(-670, 290, -3),
        5,
        5,
        30,
        "Boss Target",
        target5ModelMatrix,
        Target5Model
    );
    target5->_isBoss = 1;

    desiredRot = Player->rotation.z;



    
    

    mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);


    MainMenu = gf3d_sprite_load("images/MainMenu.png", 2400, 1400, 0);

    TextBox = gf3d_sprite_load("images/TextBox.png", 1400, 600, 0);

    Mana1 = gf3d_sprite_load("images/Mana.png", 200, 200, 0);
    Mana2 = gf3d_sprite_load("images/Mana2.png", 200, 200, 0);
    Mana3 = gf3d_sprite_load("images/Mana3.png", 200, 200, 0);
    Mana4 = gf3d_sprite_load("images/Mana4.png", 200, 200, 0);
    Mana5 = gf3d_sprite_load("images/Mana5.png", 200, 200, 0);


    Sprite* Health1 = gf3d_sprite_load("images/Health1.png", 500, 500, 0);
    Sprite* Health2 = gf3d_sprite_load("images/Health2.png", 500, 500, 0);
    Sprite* Health3 = gf3d_sprite_load("images/Health3.png", 500, 500, 0);

    Model* NPC = gf3d_model_load_M("NPC", "NPCCLOUD");
    Matrix4 NPCModelMatrix;
    gfc_matrix_identity(NPCModelMatrix);

    Entity* NPCEntity = npc_new(
        vector3d(0, 175, -8),
        50,
        50,
        30,
        "Cloud Man",
        NPCModelMatrix,
        NPC
    );
    Sprite* D1 = NULL;
    D1 = gf3d_sprite_load("images/D1.png", 1400, 600, 0);
    NPCEntity->sprite = D1;

    Model* NPC2 = gf3d_model_load_M("NPC", "NPC1");
    Matrix4 NPCModelMatrix2;
    gfc_matrix_identity(NPCModelMatrix2);
    Entity* NPCEntity2 = npc_new(
        vector3d(120, 113, -8),
        50,
        50,
        30,
        "Dangerous Man",
        NPCModelMatrix2,
        NPC2
    );
    Sprite* D2 = NULL;
    D2 = gf3d_sprite_load("images/D2.png", 1400, 600, 0);
    NPCEntity2->sprite = D2;

    Model* NPC3 = gf3d_model_load_M("NPC", "NPC1");
    Matrix4 NPCModelMatrix3;
    gfc_matrix_identity(NPCModelMatrix3);
    Entity* NPCEntity3 = npc_new(
        vector3d(240, 410, -8),
        50,
        50,
        30,
        "Whisper Man",
        NPCModelMatrix3,
        NPC3
    );
    Sprite* D3 = NULL;
    D3 = gf3d_sprite_load("images/D3.png", 1400, 600, 0);
    NPCEntity3->sprite = D3;

    Model* NPC4 = gf3d_model_load_M("NPC", "NPCSHOP");
    Matrix4 NPCModelMatrix4;
    gfc_matrix_identity(NPCModelMatrix4);
    Entity* NPCEntity4 = npc_new(
        vector3d(-140, -400, -8),
        50,
        50,
        30,
        "Shop Man",
        NPCModelMatrix4,
        NPC4
    );
    Sprite* D4 = NULL;
    D4 = gf3d_sprite_load("images/D4.png", 1400, 600, 0);
    NPCEntity4->sprite = D4;
    NPCEntity4->_isShop = 1;


    Model *DinoModel = gf3d_model_load_animated("TallCube", 1, 100);
    Matrix4 DinoModelMatrix4;
    gfc_matrix_identity(DinoModelMatrix4);
    gfc_matrix_make_translation(
        DinoModelMatrix4,
        vector3d(-250, -200, 0)
    );
    LoadConfig("config/config.json");
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_GetMouseState(&mousex, &mousey);

        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;

        if (inMainMenu == 1)
        {

            gf3d_vgraphics_render_start();
                gf3d_sprite_draw(MainMenu, vector2d(0, 0), vector2d(1, 1), 0);
            gf3d_vgraphics_render_end();
            if (keys[SDL_SCANCODE_SPACE])inMainMenu = 0;
            if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

            continue;
        }



        UpdateTime();

        gfc_matrix_make_translation(
            speedUpModelMatrix,
            vector3d(speedUpPowerUp->position.x, speedUpPowerUp->position.y, speedUpPowerUp->position.z)
        );
        gfc_matrix_make_translation(
            speedDownModelMatrix,
            vector3d(speedDownPowerUp->position.x, speedDownPowerUp->position.y, speedDownPowerUp->position.z)
        );

        gfc_matrix_make_translation(
            PlayerHandModelMatrix,
            vector3d(Player->position.x + 2, Player->position.y + 4, Player->position.z - 7)
        );

        //vector3d(Player->position.x + forward.x + 2, Player->position.y + forward.y + 4, Player->position.z + forward.z - 7)

        gfc_matrix_rotate(
            PlayerHandModelMatrix,
            PlayerHandModelMatrix,
            Player->rotation.y,
            vector3d(0, 0, 1)
        );

        //vector3d(Player->rotation.x, Player->rotation.x, Player->rotation.x),
        Player->think(Player);
        speedUpPowerUp->think(speedUpPowerUp);
        speedDownPowerUp->think(speedDownPowerUp);
        if (goldPickUp->_inuse)
            goldPickUp->think(goldPickUp);
        if (target1->_inuse)
            target1->think(target1);
        if (target2->_inuse)
            target2->think(target2);
        if (target3->_inuse)
            target3->think(target3);
        if (target4->_inuse)
            target4->think(target4);
        if (target5->_inuse)
            target5->think(target5);
        if (NPCEntity->_inuse)
            NPCEntity->think(NPCEntity);
        if (NPCEntity2->_inuse)
            NPCEntity2->think(NPCEntity2);
        if (NPCEntity3->_inuse)
            NPCEntity3->think(NPCEntity3);
        if (NPCEntity4->_inuse)
            NPCEntity4->think(NPCEntity4);
        //slog("X: %f  Y: %f  Z: %f", Player->position.x, Player->position.y, Player->position.z);
        //slog("X: %f  Y: %f  Z: %f", susPos.x, susPos.y, susPos.z);
        //slog("X: %f  Y: %f  Z: %f", rotation.x/(GFC_DEGTORAD), rotation.y / (GFC_DEGTORAD), rotation.z / (GFC_DEGTORAD));

        if (cooldown > 0)
        {
            cooldown -= deltaTime;
            if (cooldown < 0)
                cooldown = 0;
        }
        
        if (Player->rotation.z - desiredRot > -.01f && Player->rotation.z - desiredRot < .01f)
        {

        }
        else
        {
            if (Player->rotation.z < desiredRot)
                Player->rotation.z += deltaTime*10;
            else
                Player->rotation.z -= deltaTime*10;
            if (Player->rotation.z - desiredRot > -.01f && Player->rotation.z - desiredRot < .01f)
                Player->rotation.z = desiredRot;
        }
        
        PlayerBoundingBox->position = vector2d(Player->position.x, Player->position.y);
        PlayerHurtBox->position = vector2d(Player->position.x-2.5f, Player->position.y);

        if (IsColliding(SusBB, PlayerBoundingBox) == 1 && keys[SDL_SCANCODE_SPACE] && speedUpPowerUp->position.z == 0)
        {
            speedUpPowerUp->velocity.z += 100;
            gfc_sound_play(susSound, 0, 1, -1, -1);
        }


        //update game things here
        
        //gf3d_vgraphics_rotate_camera(0.001);

        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        gf3d_vgraphics_render_start();
            
            gf3d_model_draw(worldModel,RoomModelMatrix);
            gf3d_model_draw(weaponModel, PlayerHandModelMatrix);
            gf3d_model_draw(NPC, NPCModelMatrix);
            gf3d_model_draw(NPC2, NPCModelMatrix2);
            gf3d_model_draw(NPC3, NPCModelMatrix3);
            gf3d_model_draw(NPC4, NPCModelMatrix4);
            
            gf3d_model_draw_animated(DinoModel, DinoModelMatrix4,(Uint32)frame);
            frame = frame + deltaTime*60;
            if (frame >= 99)frame = 0;

            if (speedUpPowerUp->_isEquipped == 0)
                gf3d_model_draw(speedUpPowerUp->model, speedUpModelMatrix);
            if (speedDownPowerUp->_isEquipped == 0)
                gf3d_model_draw(speedDownPowerUp->model, speedDownModelMatrix);
            if (goldPickUp->_inuse)
                gf3d_model_draw(goldPickUp->model, goldPickUpModelMatrix);

            if (target1->_inuse)
                gf3d_model_draw(target1->model, targetModelMatrix);
            if (target2->_inuse)
                gf3d_model_draw(target2->model, target2ModelMatrix);
            if (target3->_inuse)
                gf3d_model_draw(target3->model, target3ModelMatrix);
            if (target4->_inuse)
                gf3d_model_draw(target4->model, target4ModelMatrix);
            if (target5->_inuse)
                gf3d_model_draw(target5->model, target5ModelMatrix);

            if (mana > 0)
                gf3d_sprite_draw(Mana1, vector2d(0, 0), vector2d(1, 1), 0);
            if (mana > 1)
                gf3d_sprite_draw(Mana2, vector2d(110, 0), vector2d(1, 1), 0);
            if (mana > 2)
                gf3d_sprite_draw(Mana3, vector2d(220, 0), vector2d(1, 1), 0);
            if (mana > 3)
                gf3d_sprite_draw(Mana4, vector2d(330, 0), vector2d(1, 1), 0);
            if (mana > 4)
                gf3d_sprite_draw(Mana5, vector2d(440, 0), vector2d(1, 1), 0);


            if (health <= 20)
                gf3d_sprite_draw(Health3, vector2d(900, 0), vector2d(1, 1), 0);
            if (health >= 21 && health < 80)
                gf3d_sprite_draw(Health2, vector2d(900, 0), vector2d(1, 1), 0);
            if (health > 80)
                gf3d_sprite_draw(Health1, vector2d(900, 0), vector2d(1, 1), 0);

            if (spriteToDraw)
                gf3d_sprite_draw(spriteToDraw, vector2d(250, 250), vector2d(1, 1), 0);

            if (UI == 1)
            {
                gf3d_sprite_draw(TextBox, vector2d(250, 250), vector2d(1, 1), 0);
                if (spriteToDraw)
                    gf3d_sprite_draw(spriteToDraw, vector2d(250, 250), vector2d(1, 1), 0);



                gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);
            }

        gf3d_vgraphics_render_end();

        if (isShopping == 1)
        {
            //slog("MouseX: %i, MouseY: %i", mousex, mousey);
        }

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

        if (keys[SDL_SCANCODE_Z])
        {
            gfc_sound_play(susSound,0,1,-1,-1);
        }
        
        float speed = 100;
        Vector3D oldPos = vector3d(Player->position.x, Player->position.y, Player->position.z);
        vector3d_set_magnitude(&forward, 0);

        if (currentDirection == 0)
        {
            //Forward
            forward = vector3d(0, 1, 0);
            vector3d_set_magnitude(&forward, deltaTime*speed);
        }
        else if (currentDirection == 1)
        {
            //Right
            forward = vector3d(-1, 0, 0);
            vector3d_set_magnitude(&forward, deltaTime* speed);
        }
        else if (currentDirection == 2)
        {
            //Backward
            forward = vector3d(0, -1, 0);
            vector3d_set_magnitude(&forward, deltaTime * speed);
        }
        else if (currentDirection == 3)
        {
            //Left
            forward = vector3d(1, 0, 0);
            vector3d_set_magnitude(&forward, deltaTime * speed);
        }
        right = vector3d(forward.y, -forward.x, 0);
        vector3d_set_magnitude(&right, deltaTime * speed * Player->speed);

        if (keys[SDL_SCANCODE_W])
            vector3d_add(Player->position, Player->position, forward);
        if (keys[SDL_SCANCODE_S])
            vector3d_add(Player->position, Player->position, -forward);
        if (keys[SDL_SCANCODE_D])
            vector3d_add(Player->position, Player->position, right);
        if (keys[SDL_SCANCODE_A])
            vector3d_add(Player->position, Player->position, -right);


        

        if (keys[SDL_SCANCODE_LSHIFT])Player->position.z -= 0.10;
        if (keys[SDL_SCANCODE_SPACE] && Player->position.z == 0)Player->velocity.z += (50 * Player->jumpHeight);

        Player->rotation.y = 180 * (GFC_DEGTORAD);


        if (isAttacking == 1)
            isAttacking = 0;

        if (cooldown == 0)
        {

            if (keys[SDL_SCANCODE_C])
            {
                slog("X: %f, Y: %f", Player->position.x, Player->position.y);
                cooldown = .3f;
            }

            if (isShopping)
            {

                //Health Button
                if (mousex > 250 && mousex < 500 && mousey > 250 && mousey < 400 && keys[SDL_SCANCODE_E])
                {
                    if (Player->gold >= 5)
                    {
                        slog("Purchase Health");
                        Player->gold -= 5;
                        health = maxHealth;
                    }
                        cooldown = .5;
                }

                //Magic Button
                if (mousex > 700 && mousex < 950 && mousey > 250 && mousey < 400 && keys[SDL_SCANCODE_E])
                {
                    if (Player->gold >= 10)
                    {
                        slog("Purchase Mana");
                        Player->gold -= 10;
                        mana = maxMana;
                    }
                    cooldown = .5;
                }
            }

            if (keys[SDL_SCANCODE_Q] && isAttacking == 0)
            {
                isAttacking = 1;
                cooldown = .1;
            }

            if (keys[SDL_SCANCODE_1])
            {
                weaponModel = AxeModel;
                damage = 1;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_2])
            {
                weaponModel = ClubModel;
                damage = 2;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_3])
            {
                weaponModel = Club2Model;
                damage = 3;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_4])
            {
                weaponModel = Club3Model;
                damage = 4;
                slog("You have %d damage", damage);
                cooldown = .1;
            }
            if (keys[SDL_SCANCODE_5])
            {
                weaponModel = Club4Model;
                damage = 5;
                slog("You have %d damage", damage);
                cooldown = .1;
            }

            if (keys[SDL_SCANCODE_KP_1])
            {
                Recall();
                cooldown = .5;
            }
            if (keys[SDL_SCANCODE_KP_2])
            {
                MegaJump();
                cooldown = .5;
            }
            if (keys[SDL_SCANCODE_KP_3])
            {
                Heal();
                cooldown = .5;
            }
            if (keys[SDL_SCANCODE_KP_4])
            {
                HealthToMana();
                cooldown = .5;
            }
            if (keys[SDL_SCANCODE_KP_5])
            {
                SummonGold();
                cooldown = .5;
            }

            int dir = 0;
            if (keys[SDL_SCANCODE_LEFT])
                dir += 1;
            if (keys[SDL_SCANCODE_RIGHT])
                dir -= 1;
            if (dir != 0)
            {
                cooldown = .1;
                int oldDir = currentDirection;
                currentDirection = changeDir(currentDirection,dir);
                if (currentDirection == 0)
                    desiredRot = 0;
                if (currentDirection == 1)
                {
                    if (oldDir == 2)
                        Player->rotation.z = 180 * (GFC_DEGTORAD);
                    desiredRot = 90 * (GFC_DEGTORAD);
                }
                if (currentDirection == 2)
                {
                    if (oldDir == 1)
                        Player->rotation.z = -270 * (GFC_DEGTORAD);
                    desiredRot = -180 * (GFC_DEGTORAD);
                }
                if (currentDirection == 3)
                    desiredRot = -90 * (GFC_DEGTORAD);
            }

            Player->rotation.x = 180 * (GFC_DEGTORAD);
        }

        PlayerBoundingBox->position = vector2d(Player->position.x, Player->position.y);

        /*
        if (IsColliding(Forward, PlayerBoundingBox) == 1 || IsColliding(Back, PlayerBoundingBox) == 1 || IsColliding(Right, PlayerBoundingBox) == 1 || IsColliding(Left, PlayerBoundingBox) == 1)
            Player->position = oldPos;
        */

        /*
        if (IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B7, PlayerBoundingBox) == 1 || IsColliding(B13, PlayerBoundingBox) == 1 ||
            IsColliding(B2, PlayerBoundingBox) == 1 || IsColliding(B8, PlayerBoundingBox) == 1 || IsColliding(B14, PlayerBoundingBox) == 1 ||
            IsColliding(B3, PlayerBoundingBox) == 1 || IsColliding(B9, PlayerBoundingBox) == 1 || IsColliding(B15, PlayerBoundingBox) == 1 ||
            IsColliding(B4, PlayerBoundingBox) == 1 || IsColliding(B10, PlayerBoundingBox) == 1 || IsColliding(B16, PlayerBoundingBox) == 1 ||
            IsColliding(B5, PlayerBoundingBox) == 1 || IsColliding(B11, PlayerBoundingBox) == 1 || IsColliding(B17, PlayerBoundingBox) == 1 ||
            IsColliding(B6, PlayerBoundingBox) == 1 || IsColliding(B12, PlayerBoundingBox) == 1 || IsColliding(B18, PlayerBoundingBox) == 1 ||
            IsColliding(B19, PlayerBoundingBox) == 1 || IsColliding(B20, PlayerBoundingBox) == 1 || IsColliding(B21, PlayerBoundingBox) == 1 ||
            IsColliding(B22, PlayerBoundingBox) == 1 || IsColliding(B23, PlayerBoundingBox) == 1 || IsColliding(B24, PlayerBoundingBox) == 1 ||
            IsColliding(B25, PlayerBoundingBox) == 1 || IsColliding(B26, PlayerBoundingBox) == 1 || IsColliding(B27, PlayerBoundingBox) == 1 ||
            IsColliding(B28, PlayerBoundingBox) == 1 || IsColliding(B29, PlayerBoundingBox) == 1
            )
*/
        if (IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B13, PlayerBoundingBox) == 1 ||
            IsColliding(B2, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B14, PlayerBoundingBox) == 1 ||
            IsColliding(B3, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B15, PlayerBoundingBox) == 1 ||
                                                       IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B16, PlayerBoundingBox) == 1 ||
            IsColliding(B5, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || 
            IsColliding(B6, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || 
                                                       IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B21, PlayerBoundingBox) == 1 ||
            IsColliding(B22, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B24, PlayerBoundingBox) == 1 ||
            IsColliding(B25, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1 || IsColliding(B27, PlayerBoundingBox) == 1 ||
            IsColliding(B28, PlayerBoundingBox) == 1 || IsColliding(B1, PlayerBoundingBox) == 1
            )
        {
            Player->position = oldPos;
        }
        
        vector3d_set_magnitude(&forward, 1);
        gf3d_camera_set_position(Player->position);
        gf3d_camera_set_rotation(Player->rotation);
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    gfc_sound_close();
    //cleanup
    slog("gf3d program end");
    slog_sync();
    gf3d_entity_manager_close();
    return 0;


}
/*eol@eof*/

int changeDir(int currentDirection, int isRight)
{
    int output = currentDirection;

    if (isRight == 1)
        output += 1;
    else
        output -= 1;

    if (output == 4)
        output = 0;
    if (output == -1)
        output = 3;

    return output;
}
