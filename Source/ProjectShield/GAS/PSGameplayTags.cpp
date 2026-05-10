// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PSGameplayTags.h"
#include "GameplayTagsManager.h"


namespace PSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ability_Guard, "Ability.Guard");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Parry, "Ability.Parry");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Deflect, "Ability.Deflect");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Roll, "Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Jump, "Ability.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Sprint, "Ability.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Ability_SprintJump, "Ability.SprintJump");

	UE_DEFINE_GAMEPLAY_TAG(State_UseRightHand, "State.UseRightHand");
	UE_DEFINE_GAMEPLAY_TAG(State_UseLeftHand, "State.UseLeftHand");
	UE_DEFINE_GAMEPLAY_TAG(State_UseBothHands, "State.UseBothHands");
	UE_DEFINE_GAMEPLAY_TAG(State_Guard, "State.Guard");
	UE_DEFINE_GAMEPLAY_TAG(State_Deflect, "State.Deflect");
	UE_DEFINE_GAMEPLAY_TAG(State_Parry, "State.Parry");
	UE_DEFINE_GAMEPLAY_TAG(State_Parry_Success, "State.Parry.Success");
	UE_DEFINE_GAMEPLAY_TAG(State_Invincible, "State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(State_StaminaRegen_Blocked, "State.StaminaRegen.Blocked");
	UE_DEFINE_GAMEPLAY_TAG(State_Roll, "State.Roll");
	UE_DEFINE_GAMEPLAY_TAG(State_Jump, "State.Jump");
	UE_DEFINE_GAMEPLAY_TAG(State_Sprint, "State.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(State_SprintJump, "State.SprintJump");
	UE_DEFINE_GAMEPLAY_TAG(State_Attack_Failed, "State.Attack.Failed");
	UE_DEFINE_GAMEPLAY_TAG(State_DisableInput, "State.DisableInput");

	UE_DEFINE_GAMEPLAY_TAG(Data_Attack, "Data.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Data_Impact, "Data.Impact");
	UE_DEFINE_GAMEPLAY_TAG(Data_ReImpact, "Data.ReImpact");
	UE_DEFINE_GAMEPLAY_TAG(Data_BreakRate, "Data.BreakRate");
	UE_DEFINE_GAMEPLAY_TAG(Data_DefenseType, "Data.DefenseType");
	UE_DEFINE_GAMEPLAY_TAG(Data_MoveSpeed, "Data.MoveSpeed");

	UE_DEFINE_GAMEPLAY_TAG(Event_Roll_Start, "Event.Roll.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Parry_Start, "Event.Parry.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Deflect_Start, "Event.Deflect.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Start, "Event.Attack.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_End, "Event.Attack.End");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Hit, "Event.Attack.Hit");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_Hit_Bleed, "GameplayCue.Combat.Hit.Bleed");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_Hit_Spark, "GameplayCue.Combat.Hit.Spark");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_Hit_Parry, "GameplayCue.Combat.Hit.Parry");
}