// Copyright Alexander Terentyev. All Rights Reserved.

#include "CosmicTrespassersNativeTags.h"

//~=============================================================================
// Messages
UE_DEFINE_GAMEPLAY_TAG(TAG_Message_Channel_Game_Phase, "Message.Channel.GamePhase");

//~=============================================================================
// Game States
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Entered, "GamePhase.Entered");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Exited, "GamePhase.Exited");

UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Splash, "GamePhase.Splash");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_StartMenu, "GamePhase.StartMenu");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_SpawnEnemies, "GamePhase.SpawnEnemies");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Gameplay, "GamePhase.Gameplay");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Game_Over, "GamePhase.GameOver");
UE_DEFINE_GAMEPLAY_TAG(TAG_Game_Phase_Pause, "GamePhase.Pause");

//~=============================================================================
// Ship
UE_DEFINE_GAMEPLAY_TAG(TAG_Ship, "Ship")

UE_DEFINE_GAMEPLAY_TAG(TAG_Ship_Ability_Attack, "Ship.Ability.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ship_Ability_Attack_Cooldown, "Ship.Ability.Attack.Cooldown")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ship_Ability_Destroy, "Ship.Ability.Destroy")

//~=============================================================================
// Enemies
UE_DEFINE_GAMEPLAY_TAG(TAG_Enemy, "Enemy")
UE_DEFINE_GAMEPLAY_TAG(TAG_Enemy_Bonus, "Enemy.Bonus")

UE_DEFINE_GAMEPLAY_TAG(TAG_Enemy_Ability_Attack, "Enemy.Ability.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Enemy_Ability_Destroy, "Enemy.Ability.Destroy")

//~=============================================================================
// Bunker
UE_DEFINE_GAMEPLAY_TAG(TAG_Bunker, "Bunker")
UE_DEFINE_GAMEPLAY_TAG(TAG_Bunker_Ability_TakeDamage, "Bunker.Ability.TakeDamage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Bunker_Ability_TakeDamage_Trigger, "Bunker.Ability.TakeDamage.Trigger")
UE_DEFINE_GAMEPLAY_TAG(TAG_Bunker_Ability_DestroyPart, "Bunker.Ability.DestroyPart")
UE_DEFINE_GAMEPLAY_TAG(TAG_Bunker_Ability_DestroyPart_Trigger, "Bunker.Ability.DestroyPart.Trigger")

//~=============================================================================
// Projectile
UE_DEFINE_GAMEPLAY_TAG(TAG_Projectile, "Projectile")
UE_DEFINE_GAMEPLAY_TAG(TAG_Projectile_Ability_Hit, "Projectile.Ability.Hit")
UE_DEFINE_GAMEPLAY_TAG(TAG_Projectile_Ability_Hit_Trigger, "Projectile.Ability.Hit.Trigger")
UE_DEFINE_GAMEPLAY_TAG(TAG_Projectile_Destroy, "Projectile.Ability.Destroy")