
//**********************************************************************
// PL_StandStill: Just stand there.
//**********************************************************************

PriorityList PL_StandStill
{
	Priority StandStill
	{
		Action: actionStandStill
	}
}

//**********************************************************************
// PL_NONE: Do Nothing
//**********************************************************************

PriorityList PL_NONE
{
	Priority Nothing
	{
		Event ExitCombat
		{
			AtTime:		0
			Action:		actionExitCombat
		}
	}
}

//**********************************************************************
// PL_UseAttackPower: Use this power on your currect target right now
//**********************************************************************

PriorityList PL_UseBasicAttackPowerNow
{
	Priority UseAttackPower
	{
		Event UseAttackPower
		{
			AtTime:		0
			Action
			{
				Type:		actionUsePowerNow
				PowerName:	FlameThrower
			}
		}
	}
}

//**********************************************************************
// PL_UsePowerNow: Use current power on your currect target right now
//**********************************************************************

PriorityList PL_UsePowerNow
{
	Priority UsePowerNow
	{
		Event UsePowerNow
		{
			AtTime:		0
			Action
			{
				Type:		actionUsePowerNow
			}
		}
	}
}

//**********************************************************************
// PL_Invisible: Not seen by clients.
//**********************************************************************

PriorityList PL_Invisible
{
	Priority Invisible
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace
	}
}

//**********************************************************************
// PL_FleeToNearestDoor: Run to nearest door and disappear.
//**********************************************************************

PriorityList PL_FleeToNearestDoor
{
	Priority FleeToNearestDoor
	{
		Action:	actionRunIntoDoor
	}
}

//**********************************************************************
// PL_FleeAwayNearest: Run to nearest door and disappear.
//**********************************************************************

PriorityList PL_FleeAwayNearest
{
	Priority FleeToNearestDoor
	{
		Action
		{
			Type:		actionRunToTarget
			TargetType:	targetFarthestBeacon
		}

		Event Destroy
		{
			AtTime: 90

			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_VictimFleeNearest: Run away, but only to a short distance.
//**********************************************************************

PriorityList PL_VictimFleeNearest
{
	Priority Cower
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear"
		}

		Event StopCowering
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}

	Priority SneakAway
	{
		Action
		{
			Type:		actionRunAway
			AnimBits:	"fear", "sneak"
			DoNotRun:	true
			TargetType:	targetNearbyCritters
			Radius:		150
		}

		Event StopSneaking
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}
	
	Priority RunAway
	{
		Action
		{
			Type:		actionRunAway
			TargetType:	targetNearbyCritters
			Radius:		150
		}
	}
}

//**********************************************************************
// PL_VictimFleeClose: Run away, but only to a short distance.
//**********************************************************************

PriorityList PL_VictimFleeClose
{
	Priority Cower
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear"
		}

		Event StopCowering
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}

	Priority SneakAway
	{
		Action
		{
			Type:		actionRunAway
			AnimBits:	"fear", "sneak"
			DoNotRun:	true
			TargetType:	targetNearbyCritters
			Radius:		150
		}

		Event StopSneaking
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}
	
	Priority RunAway
	{
		Action
		{
			Type:		actionRunAway
			TargetType:	targetNearbyCritters
			Radius:		150
		}
	}
}

//**********************************************************************
// PL_FrozenInPlace: Sit there frozen (no physics, no nothing).
//**********************************************************************

PriorityList PL_FrozenInPlace
{
	Priority FrozenInPlace
	{
		Action: actionFrozenInPlace
	}
}

//**********************************************************************
// PL_GoodFrozenInPlace: Sit there frozen (no physics, no nothing).
//**********************************************************************

PriorityList PL_GoodAndFrozenInPlace
{
	Priority FrozenInPlace
	{
		Action: actionFrozenInPlace
		Set: me.onTeamGood = true
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = false
	}
}

//**********************************************************************
// PL_FrozenInPlace: Sit there frozen (no physics, no nothing).
//**********************************************************************

PriorityList PL_FoulAndFrozenInPlace
{
	Priority FrozenInPlace
	{
		Action: actionFrozenInPlace
		Set: me.onTeamGood = false
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = true
	}
}

//**********************************************************************
// PL_Run: Always run away.
//**********************************************************************

PriorityList PL_Run
{
	Priority RunAway
	{
		Action: actionCombat
		Set:	me.feeling.fear = 20
		Set:	me.feeling.confidence = 10
	}
}

//**********************************************************************
// PL_DestroyMe
//**********************************************************************

PriorityList PL_DestroyMe
{
	Priority DestroyMe
	{
		Set: me.destroy = true
	}
}

//**********************************************************************
// PL_TrainInStation
//**********************************************************************

PriorityList PL_TrainInStation
{
	Priority EnterStation
	{
		HoldPriority: true
		
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	train, yes
		}

		Event HangAround
		{
			AtTime: 1

			Action:	actionNextPriority
		}
	}

	Priority HangAround
	{
		HoldPriority: true
		
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	train
		}

		Event HangAround
		{
			AtTime: 20

			Action:	actionNextPriority
		}
	}

	Priority LeaveStation
	{
		HoldPriority: true
		
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	train, close
		}

		Event HangAround
		{
			AtTime: 1

			Action:	actionNextPriority
		}
	}

	Priority HangAround
	{
		HoldPriority: true
		
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	train
		}

		Event HangAround
		{
			AtTime: 20

			Action:	actionNextPriority
		}
	}
}


