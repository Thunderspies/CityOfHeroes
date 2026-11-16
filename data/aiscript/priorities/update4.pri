// EJ's experimental stuff, please don't modify

//**********************************************************************
// PL_Chant_Then_Follow_Route: Template to have actor perform one animation for a set amount of time, 
// then go into "Follow" (Patrol) mode. 
//**********************************************************************

PriorityList PL_Chant_Then_Follow_Route
{
	Priority Cast
	{
		HoldPriority: true
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast"
		}
		
		Event Done
		{
			AtTime:		20
			Action:		actionNextPriority
		}
		
	}
	Priority Follow
	{
		HoldPriority: true
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	0
		}
	}
}

//**********************************************************************
// PL_Rise_From_Ground_Delay:  Causes Plantman to spawn as tree, but stay out of aggro for 15 secs
//**********************************************************************

PriorityList PL_Rise_From_Ground_Delay
{
	Priority PlantmanAmbush
	{
		HoldPriority: true
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Groundsummon"
		}
		Event Done
		{
			AtTime:		15
			Action:		actionNextPriority
		}
	}
	Priority Combat
		{
			HoldPriority: true
			Action: actionCombat
		}
}

//**********************************************************************
// PL_Rise_From_Water_Follow_Route:  Rises from the water, then begins following a set path
//**********************************************************************

PriorityList PL_Rise_From_Water_Follow_Route
{
	Priority Rise_From_Water
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Groundsummon", "Low"
		}

		Event Done
		{
			AtTime:		1
			Action:		actionNextPriority
		}
		
	}
	Priority Follow
	{
		HoldPriority: true
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}
	}
}

//**********************************************************************
// PL_Follow_Route_Wire_Flying:  Flying Patrol with no clipping, perfect for ghostly haunts
//**********************************************************************

PriorityList PL_Follow_Route_Wire_Flying
{
	Priority Follow_Route_Wire
	{
		Set:		me.overridePerceptionRadius = 10
		Set:		me.flying = true
		Action
		{
			Type:		actionFollowRoute
			TargetType: targetWire
			doNotRun:	0
		}
	}
}

//**********************************************************************
// PL_Follow_Route_Wire_Untargetable:  Flying Patrol with no clipping, and untargetable
//**********************************************************************

PriorityList PL_Follow_Route_Wire_Untargetable
{
	Priority Follow_Route_Wire
	{
		Set:		me.overridePerceptionRadius = 10
		Set:		me.flying = true
		Set:		me.untargetable = true
		Action
		{
			Type:		actionFollowRoute
			TargetType: targetWire
			doNotRun:	0
		}
	}
}

//**********************************************************************
// PL_Plantman_Pumpkin_Ambush_NPC_Cleanup
//**********************************************************************

PriorityList PL_Plantman_Pumpkin_Ambush_NPC_CleanUp
{
	Priority Plantman_Pumpkin_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Magic", "Low"
		}
		Event AddVariationBit
		{
			AtTime:	20
			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_WalkingTrees: Spawns in tree disguise, goes into patrol mode, then back to tree. 
//**********************************************************************

PriorityList PL_WalkingTrees
{
	Priority PlantmanAmbush
	{
		HoldPriority: true
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "High"
		}
		
		Event Done
		{
			AtTime:		5
			Action:		actionNextPriority
		}
		
	}
	Priority Follow
	{
		HoldPriority: true
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}

		Event Done
		{
			AtTime:		10
			Action:		actionNextPriority
		}

	}
}

//**********************************************************************
// PL_Loiter_Untargetable: Stand around looking like a punk, and be untargetable.
//**********************************************************************

PriorityList PL_Loiter_Untargetable
{
	Priority WaitForAWhile
	{
		HoldPriority: true
		Set:		me.untargetable = true
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
		Set:		me.untargetable = true

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
// PL_UseAIConfigThenDie
//**********************************************************************

PriorityList PL_UseAIConfigThenDie
{
	Priority Combat
	{
		Action: actionCombat
		Event AddVariationBit
		{
			AtTime:	60
			Set: me.destroy = true
			Set: me.untargetable = false
		}
	}
}

//**********************************************************************
// PL_Follow_Route_Wire_Then_Die:  Follow a path then die
//**********************************************************************

PriorityList PL_Follow_Route_Wire_Then_Die
{
	Priority Follow_Route_Wire
	{
		Set:		me.overridePerceptionRadius = 10
		Set:		me.flying = true
		Set:		me.untargetable = true
		Action
		{
			Type:		actionFollowRoute
			TargetType: targetWire
			doNotRun:	0
		}
		Event Done
		{
			AtTime: 5
			Set: me.destroy = true
		}
	}
}
//**********************************************************************
// PL_Rock_Rampage_Delay: Adds variation to existing PL_RockRampage
//**********************************************************************

PriorityList PL_Rock_Rampage_Delay
{
	Priority Rampage
	{
		HoldPriority: true
		Set:		me.untargetable = true
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "High"
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
	
	Priority Howl
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Yell", "Strong"
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

	Priority Surrounded
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Strong", "Menace", "Lookout"
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
// PL_FloatingStruggleEvil: Used for NPC floating in the air, being held magically (Team Evil)
//**********************************************************************

PriorityList PL_FloatingStruggleEvil
{
	Priority Float
	{
		Set: me.onTeamEvil = true
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast", "Fear"
		}
	}
}

//**********************************************************************
// PL_UseAIConfigFoul
//**********************************************************************

PriorityList PL_UseAIConfigFoul
{
	Priority Combat
	{
		Set: me.onTeamFoul = true
		Set: me.untargetable = false
		Action: actionCombat
	}
}

//**********************************************************************
// PL_UseAIConfigEvil
//**********************************************************************

PriorityList PL_UseAIConfigEvil
{
	Priority Combat
	{
		Set: me.onTeamEvil = true
		Set: me.untargetable = false
		Action: actionCombat
	}
}

//**********************************************************************
// PL_Vampire_Crouch_Foul: 
//**********************************************************************

PriorityList PL_Vampire_Crouch_Foul
{
	
	Priority Vampire_Crouch
	{		
		HoldPriority: true
		Set: me.onTeamFoul = true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Busy", "Pose", "Magic", "Low"
		}

	}
}


