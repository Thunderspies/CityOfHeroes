
//**********************************************************************
// PL_DefaultCritter
//**********************************************************************

PriorityList PL_DefaultCritter
{
	Priority Attack
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_DefaultCritter
//**********************************************************************
PriorityList PL_EscapedGhosts
{

	Priority Wait
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RECLAIM", "EMERGE", "RAISEDEAD"
		}

		Event Done
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority Attack
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_DeathFromAbove: Villain will remain in place hidden from players and untargetable for 100 seconds, 
//			or when players get into aggro range, whichever comes first, then drop down and attack.
//**********************************************************************

PriorityList PL_DeathFromAbove
{
	Priority Wait
	{
		Set: me.doNotDrawOnClient = true

		Action: actionFrozenInPlace

		Event Done
		{
			AtTime: 100
			Action: actionDisablePriority
		}
	}

	Priority Attack
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_MitoHang
//**********************************************************************

PriorityList PL_MitoHang
{
	Priority Wait
	{
		Set: me.doNotDrawOnClient = true

		Action: actionFrozenInPlace

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Menace"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}

		Event Done
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority Attack
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_Loiter: Stand around looking like a punk.
//**********************************************************************

PriorityList PL_Loiter
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}

//**********************************************************************
// PL_Wander: 
//**********************************************************************

PriorityList PL_Wander
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 4
			Action: actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}


//**********************************************************************
// PL_Loiter_Small: 
//**********************************************************************

PriorityList PL_Loiter_Small
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		20
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}



//**********************************************************************
// PL_DanceAndFight: 
//**********************************************************************

PriorityList PL_DanceAndFight
{

	Priority Wander
	{	
		Condition: me.hasAttackTarget
		Action: actionCombat
	}

	Priority Wander
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Dance", "Low", "Pose"
		}
	}
}


//**********************************************************************
// PL_WanderOff: 
//**********************************************************************

PriorityList PL_WanderOff
{

	Priority Wander
	{	
		Condition: me.hasAttackTarget
		Action: actionCombat
		Set:		me.spawnPosIsSelf = true
	}

	Priority Wander
	{	
		Set:		me.spawnPosIsSelf = true
		Action
		{
			Type:		actionWander
			Radius:		1000
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}
	}
}

//**********************************************************************
// PL_LoiterScavenge - loiter around, but stop and look at the ground
//                     every now and then
//**********************************************************************

PriorityList PL_LoiterScavenge
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loot"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}

	Priority StandUp
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		3
			Action:		actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}

//**********************************************************************
// PL_LoiterLookout - loiter around, but stop and look at the ground
//                     every now and then
//**********************************************************************

PriorityList PL_LoiterLookout
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout"
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		5
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 6
			Action: actionNextPriority
		}
	}

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter"
		}

		Event Done
		{
			AtTime:		5
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		7
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 10
			Action: actionNextPriority
		}
	}
}

//**********************************************************************
// PL_FleeAwayFar
//**********************************************************************

PriorityList PL_FleeAwayFar
{
	Priority RunAway
	{		
		Action: actionCombat
		Set:	me.feeling.fear = 20
		Set:	me.feeling.confidence = 10
		Set:	me.feeling.loyalty = 0
	}
}

//**********************************************************************
// PL_FightThenRun: Attack, then run away at some point.
//**********************************************************************

PriorityList PL_FightThenRun
{
	Priority Attack
	{		
		Condition: me.healthPercent > 30
		Action: actionCombat
	}

	Priority RunAway
	{
		Action: actionCombat
		Set:	me.feeling.fear = 20
		Set:	me.feeling.confidence = 10
		Set:	me.feeling.loyalty = 0
	}
}

//**********************************************************************
// PL_Snipe: Shoot anyone within range.
//**********************************************************************

PriorityList PL_Snipe
{
	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}


//**********************************************************************
// PL_Willforge: Shoot anyone within range.
//**********************************************************************

PriorityList PL_Willforge
{
	Priority Snipe
	{
		Event Init
		{
			AtTime: 0
			Set:	me.untargetable = true
			Set:	me.flying = true
		}
		Set: me.doNotMove = true

		Action: actionFrozenInPlace
		//Action: actionCombat
	}
}


//**********************************************************************
// PL_FightMeleeToDeath: Always melee attack.
//**********************************************************************

PriorityList PL_FightMeleeToDeath
{
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_UseAIConfig
//**********************************************************************

PriorityList PL_UseAIConfig
{
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_BeFoulAndUseAIConfig
//**********************************************************************

PriorityList PL_BeFoulAndUseAIConfig
{
	Priority Combat
	{
		Set: me.onTeamFoul = true
		Action: actionCombat
			
	}
}

//**********************************************************************
// PL_GoodTurret
//**********************************************************************

PriorityList PL_GoodTurret
{
	Priority Combat
	{
		Set: me.onTeamGood = true
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = false
		Action: actionCombat
	}
}

//**********************************************************************
// PL_FoulTurret
//**********************************************************************

PriorityList PL_FoulTurret
{
	Priority Combat
	{
		Set: me.onTeamGood = false
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = true
		Action: actionCombat
	}
}

//**********************************************************************
// PL_EvilTurret
//**********************************************************************

PriorityList PL_EvilTurret
{
	Priority Combat
	{
		Set: me.onTeamGood = false
		Set: me.onTeamEvil = true
		Set: me.onTeamFoul = false
		Action: actionCombat
	}
}

//**********************************************************************
// PL_UseAIConfig_SpawnFlying
//**********************************************************************

PriorityList PL_UseAIConfig_SpawnFlying
{
	Priority Combat
	{
		Action: actionCombat

		Event Init
		{
			AtTime: 0
			Set:	me.flying = true
		}
	}
}

//**********************************************************************
// PL_FightPreferMelee
//**********************************************************************

PriorityList PL_FightPreferMelee
{
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_FightRangedToDeath: Always ranged attack.
//**********************************************************************

PriorityList PL_FightRangedToDeath
{
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_FightPreferRanged
//**********************************************************************

PriorityList PL_FightPreferRanged
{
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_Follow_Route etc
//**********************************************************************

PriorityList PL_Follow_Route
{
	Priority Combat
	{
		Set:		me.spawnPosIsSelf = true
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority Follow_Route
	{
		Set:		me.overridePerceptionRadius = 10
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}
	}
}

PriorityList PL_Follow_Route_Run
{
	Priority Combat
	{
		Set:		me.spawnPosIsSelf = true
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority Follow_Route_Run
	{
		Set:		me.overridePerceptionRadius = 10
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	0
		}
	}
}

PriorityList PL_Follow_Route_Fly
{
	Priority Combat
	{
		Set:		me.spawnPosIsSelf = true
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority Follow_Route_Fly
	{
		Set:		me.overridePerceptionRadius = 10
		Set:		me.flying = true
		Action
		{
			Type:		actionFollowRoute
		}
	}
}

PriorityList PL_Follow_Route_Wire
{
	Priority Follow_Route_Wire
	{
		Action
		{
			Type:		actionFollowRoute
			TargetType: targetWire
			doNotRun:	0
		}
	}
}

PriorityList PL_Follow_Route_No_Combat
{
	Priority Follow_Route
	{
		Set:		me.overridePerceptionRadius = 10
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}
	}
}
//**********************************************************************
// PL_Untargetable_FightPreferRanged
//**********************************************************************

PriorityList PL_Untargetable_FightPreferRanged
{
	Priority Attack
	{
		Action:	actionCombat

		Event Init
		{
			AtTime: 0
			Set:	me.untargetable = true
		}
	}
}

//**********************************************************************
// PL_Untargetable_SpawnFlying
//**********************************************************************

PriorityList PL_Untargetable_SpawnFlying
{
	Priority Attack
	{
		Action:	actionCombat

		Event Init
		{
			AtTime: 0
			Set:	me.untargetable = true
			Set:	me.flying = true
		}
	}
}

//**********************************************************************
// PL_TargetInstigator
//**********************************************************************

PriorityList PL_TargetInstigator 
{
	Priority Attack
	{
		Set:	me.targetInstigator = true
		Set:	me.feeling.bothered = 100
		Action:	actionCombat
	}
}

//**********************************************************************
// PL_Aggressive
//**********************************************************************

PriorityList PL_Aggressive
{
	Priority Attack
	{
		Set:	me.feeling.bothered = 100
		Action:	actionCombat
		Action: actionNeverForgetAttackTarget
	}
}

//**********************************************************************
// PL_AggressiveFlying
//**********************************************************************

PriorityList PL_AggressiveFlying
{
	Priority Attack
	{
		Event StartFlying
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Set:	me.feeling.bothered = 100
		Action:	actionCombat
		Action: actionNeverForgetAttackTarget
	}
}

//**********************************************************************
// PL_SpawnFlying
//**********************************************************************

PriorityList PL_SpawnFlying
{
	Priority Attack
	{
		Event StartFlying
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Action:	actionCombat
	}
}

//**********************************************************************
// PL_Patrol -	Agents independently move to randomly-selected patrol points on the map. 
//		Once patrol point is reached, agent will wait for a while and then move on to the next patrol point.
//		If the agent encounters a combat target, it will drop out of patrol mode and fight.
//		After combat is over, AI will wait for a while, and then resume patrol behavior.
//**********************************************************************

PriorityList PL_Patrol
{
	Priority Fight
	{
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority WaitAfterCombat
	{
		Condition:	me.timeSinceCombat < 10
		Action:		actionCombat
	}

	Priority ChoosePatrolPoint
	{
		Condition:	me.hasPatrolPoint == false
		Action:		actionChoosePatrolPoint
	}

	Priority GoToPatrolPoint
	{
		Condition: me.hasReachedTarget == false

		Action
		{
			Type:		actionPatrol
			AnimBits:	"Loiter"
		}
	}

	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"loiter"
		}

		Event Fight
		{
			Condition:	me.hasAttackTarget
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		8
			Set:		me.hasPatrolPoint = false
			Action:		actionNextPriority
		}
	}
}

//**********************************************************************
// PL_Patrol_Group -	Move to randomly-selected patrol points on the map.
//			All members follow their leader.
//			The entire team is set to "alerted" state.
//			Once patrol point is reached, agent will wait for a while and then move on to the next patrol point.
//			If the team encounters a combat target, it will drop out of patrol mode and fight.
//			After combat is over, AI will wait for a while, and then resume patrol behavior.
//**********************************************************************

PriorityList PL_Patrol_Group
{
	Priority Alert
	{
		Set:		me.hasPatrolPoint = false
		Set:		me.spawnPosIsSelf = true
		Action:		actionDisablePriority
		Action:		actionAlertTeam
	}

	Priority Fight
	{
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority WaitAfterCombat
	{
		Condition:	me.timeSinceCombat < 10
		Action:		actionCombat
	}


	Priority FollowLeader
	{
		Condition:	me.isLeader == false
		Action:		actionFollowLeader
	}

	Priority ChoosePatrolPoint
	{
		Condition:	me.isLeader
		Condition:	me.hasPatrolPoint == false
		Action
		{
			Type:		actionChoosePatrolPoint
			AnimBits:	"loiter"
		}
	}

	Priority GoToPatrolPoint
	{
		Condition: me.isLeader
		Condition: me.hasReachedTarget == false

		Action
		{
			Type:		actionPatrol
			AnimBits:	"loiter"
		}
	}

	Priority WaitForAWhile
	{
		HoldPriority: true
		Condition: me.isLeader

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"loiter"
		}

		Event Fight
		{
			Condition:	me.hasAttackTarget
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		8
			Set:		me.hasPatrolPoint = false
			Action:		actionNextPriority
		}
	}
}

//**********************************************************************
// PL_InvisibleTarget
//**********************************************************************

PriorityList PL_InvisibleTarget
{
	Priority WaitForAttackers
	{
		Action: actionFrozenInPlace

		Event MakeAttackable
		{
			AtTime: 0

			Set: me.findTarget = false
			Set: me.invincible = true
			Set: me.alwaysGetHit = true
			Set: me.doNotDrawOnClient = true
			Set: me.onTeamGood = true
			Set: me.onTeamEvil = false
		}

		Event Next
		{
			// Give 10 seconds to start attacking.

			AtTime: 10
			Action: actionDisablePriority
		}

		Event Next
		{
			Condition: me.attackerCount != 0
			Action: actionDisablePriority
		}
	}
	
	Priority WaitForNoAttackers
	{
		Action: actionFrozenInPlace

		Event MakeAttackable
		{
			AtTime: 0

			Set: me.findTarget = false
			Set: me.invincible = true
			Set: me.alwaysGetHit = true
			Set: me.doNotDrawOnClient = true
			Set: me.onTeamGood = true
			Set: me.onTeamEvil = false
		}

		Event Destroy
		{
			// Destroy me when no one is attacking me anymore.

			Condition: me.attackerCount == 0
			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_TimedPowersTest
//**********************************************************************

PriorityList PL_TimedPowersTest
{
	Priority DoEverything
	{
		Event 80-90
		{
			Condition: me.healthPercent < 90
			Condition: me.healthPercent >= 80
			Action.SetPowerOn:			Lightning_Field
			ElseAction.SetPowerOff:		Lightning_Field
		}

		Event 80-90
		{
			Condition: me.healthPercent < 80
			Condition: me.healthPercent >= 70
			Action.SetPowerOn:			Lightning_Field
			ElseAction.SetPowerOff:		Lightning_Field
		}
	}
}


//**********************************************************************
// PL_RunOutOfDoor
//**********************************************************************

PriorityList PL_RunOutOfDoor
{
	Priority RunOutOfDoor
	{
		Event RunOut
		{
			attime: 0
			Action: actionRunOutOfDoor
		}
		Event next
		{
			Condition: me.outOfDoor
			Action: actionDisablePriority
		}
	}
	Priority Fight
	{
		Action: actionCombat
	}
}


//**********************************************************************
// PL_RunOutOfDoor_Then_Follow_Route
//**********************************************************************

PriorityList PL_RunOutOfDoor_Then_Follow_Route
{
	Priority RunOutOfDoor
	{
		Event RunOut
		{
			attime: 0
			Action: actionRunOutOfDoor
		}
		Event next
		{
			Condition: me.outOfDoor
			Action: actionDisablePriority
		}
	}

	Priority Combat
	{
		Set:		me.spawnPosIsSelf = true
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority Follow_Route
	{
		Set:		me.overridePerceptionRadius = 10
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}
	}
}

//**********************************************************************
// PL_RunOutOfDoor_Then_Follow_Route_Run
//**********************************************************************

PriorityList PL_RunOutOfDoor_Then_Follow_Route_Run
{
	Priority RunOutOfDoor
	{
		Event RunOut
		{
			attime: 0
			Action: actionRunOutOfDoor
		}
		Event next
		{
			Condition: me.outOfDoor
			Action: actionDisablePriority
		}
	}

	Priority Combat
	{
		Set:		me.spawnPosIsSelf = true
		Condition:	me.hasAttackTarget
		Action:		actionCombat
	}

	Priority Follow_Route_Run
	{
		Set:		me.overridePerceptionRadius = 10
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	0
		}
	}
}

///////////////////////////////////////////////////////////


//**********************************************************************
// PL_BallistaCallForHelp: Call for help to all immediate area Longbow Spec-Ops
//**********************************************************************

PriorityList PL_BallistaCallForHelp
{
	Priority Call
	{
		Action: actionCombat

		Event GetHelp
		{
			Condition: me.timeSinceWasAttacked < 60

			//And Summon all monsters within 300 feet
			Action
			{
				Type: actionCallForHelp
				TargetName: "Longbow Spec-Ops"
				Radius: 300
			}
		Action:  actionDisablePriority
		}
	}
	Priority Fight
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_Longbow_Stealth: Enable Stealth for Longbow Spec-Ops
//**********************************************************************

PriorityList PL_Longbow_Stealth
{
	Priority UsePowerNow
	{
		Set:		me.overridePerceptionRadius = 10
		Event UsePowerNow
		{
			AtTime:		0
			Action.SetPowerOn:			Cloaking_Device
		}
	}
}
////////////////////////////////////////////////////////////////////////


PriorityList PL_RockWallCallForHelp
{
	Priority Call
	{
		Condition: me.healthPercent < 70

		//Summon all the monsters named Quarry within 2000 feet
		Action
		{
			Type: actionCallForHelp
			TargetName: Quarry
			Radius: 2000
		}
		//And Summon all monsters within 500 feet
		Action
		{
			Type: actionCallForHelp
			Radius: 500
		}
		Action:  actionDisablePriority
	}

	Priority Fight
	{
		Action: actionCombat
	}
}


PriorityList PL_EggsCallForHelp
{
	Priority Call
	{
		Condition: me.healthPercent < 70

		//Summon all the monsters named Quarry within 2000 feet
		Action
		{
			Type: actionCallForHelp
			TargetName: Snakes_Stheno
			Radius: 2000
		}
		Action:  actionDisablePriority
	}

	Priority Fight
	{
		Action: actionCombat
	}
}


PriorityList PL_CallForHelp
{
	Priority Call
	{
		Condition: me.healthPercent < 70

		//And Summon all monsters within 500 feet
		Action
		{
			Type: actionCallForHelp
			Radius: 500
		}
		Action:  actionDisablePriority
	}

	Priority Fight
	{
		Action: actionCombat
	}
}


PriorityList PL_CallForHelpWhenAttacked
{
	Priority Call
	{
		Action: actionFrozenInPlace

		Event GetHelp
		{
			Condition: me.timeSinceWasAttacked < 60

			//And Summon all monsters within 500 feet
			Action
			{
				Type: actionCallForHelp
				Radius: 500
			}
		}
	}
}


#PriorityList PL_CallForHelp
#{
#            Priority Call
#            {
#                        Condition: me.healthPercent < 50
#						Action
#						{
#							Type: actionCallForHelp
#							Radius: 500
#						}
#                        Action:  actionDisablePriority
#            }
# 
#            Priority Fight
#            {
#						Event CallForHelp
#						{
#							AtTime: 5
#							Action
#							{
#								Type: actionCallForHelp
#								Radius: 500
#							}
#							Action: actionNextPriority
#						}
#                        Action: actionCombat
#            }
#}
#



//**********************************************************************
// PL_ThankHero: Run up to hero who "saved" me.
//**********************************************************************

PriorityList PL_ThankAndRun
{
	Priority DoEverything
	{
		Action: actionNPC.ThankHero
	}
}

//**********************************************************************
// PL_ScriptControlledActivity: Used by AIScript to run its own activities
//**********************************************************************

PriorityList PL_ScriptControlledActivity
{
	Priority ScriptControlledActivity
	{
		Action: actionScriptControlledActivity
	}
}

//**********************************************************************
// PL_ScriptControlledActivity: Used by AIScript to run its own activities
//**********************************************************************

PriorityList PL_HideBehindEnt
{
	Priority PL_HideBehindEnt
	{
		Action: actionHideBehindEnt
	}
}

PriorityList PL_LimitedSpeed
{
	Priority Combat
	{
		Set:	me.limitedSpeed = 150
		Action: actionCombat
	}
}