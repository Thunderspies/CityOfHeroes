

PriorityList PL_BallTurret
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event Next
		{
			Condition: me.hasAttackTarget
			Action: actionDisablePriority
			Action
			{
				Type:		actionSetTeammateAnimBits
				TargetName: "Turrets_PopUp_Turret_Base"
				AnimBits:	COMBAT, WEAPON, GUN, MACHINEGUN
				Radius:		3
			}
		}
	}

	Priority WaitForSlideOut
	{
		Action
		{
			Type: actionFrozenInPlace
			AnimBits: COMBAT, WEAPON, GUN, MACHINEGUN
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

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event RaiseBase
		{
			Condition: me.hasAttackTarget
		}
	}
}

PriorityList PL_BallTurretBase
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event Next
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority DieWhenNoBall
	{
		Action: actionFrozenInPlace

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event DestroyMe
		{
			NotCondition
			{
				Operator:	opHasTeammateNearby
				Param1:		"Turrets_PopUp_Turret"
				Param2:		3
			}

			Set: me.destroy = true
		}
	}
}


///////////////////////////////////////////////////////////////
// PLs for Good SafeZones

PriorityList PL_GoodBallTurret
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Set: me.onTeamGood = true
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = false

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event Next
		{
			Condition: me.hasAttackTarget
			Action: actionDisablePriority
			Action
			{
				Type:		actionSetTeammateAnimBits
				TargetName: "Turrets_PopUp_Turret_Base"
				AnimBits:	COMBAT, WEAPON, GUN, MACHINEGUN
				Radius:		3
			}
		}
	}

	Priority WaitForSlideOut
	{
		Action
		{
			Type: actionFrozenInPlace
			AnimBits: COMBAT, WEAPON, GUN, MACHINEGUN
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

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event RaiseBase
		{
			Condition: me.hasAttackTarget
		}
	}
}

PriorityList PL_GoodBallTurretBase
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Set: me.onTeamGood = true
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = false

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event Next
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority DieWhenNoBall
	{
		Action: actionFrozenInPlace

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event DestroyMe
		{
			NotCondition
			{
				Operator:	opHasTeammateNearby
				Param1:		"Turrets_PopUp_Turret"
				Param2:		3
			}

			Set: me.destroy = true
		}
	}
}

///////////////////////////////////////////////////////////////
// PLs for Foul SafeZones

PriorityList PL_FoulBallTurret
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Set: me.onTeamGood = false
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = true

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event Next
		{
			Condition: me.hasAttackTarget
			Action: actionDisablePriority
			Action
			{
				Type:		actionSetTeammateAnimBits
				TargetName: "Turrets_PopUp_Turret_Base"
				AnimBits:	COMBAT, WEAPON, GUN, MACHINEGUN
				Radius:		3
			}
		}
	}

	Priority WaitForSlideOut
	{
		Action
		{
			Type: actionFrozenInPlace
			AnimBits: COMBAT, WEAPON, GUN, MACHINEGUN
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

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}

		Event RaiseBase
		{
			Condition: me.hasAttackTarget
		}
	}
}

PriorityList PL_FoulBallTurretBase
{
	Priority Wait
	{
		Action: actionFrozenInPlace

		Set: me.onTeamGood = false
		Set: me.onTeamEvil = false
		Set: me.onTeamFoul = true

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event Next
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority DieWhenNoBall
	{
		Action: actionFrozenInPlace

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
			Set:	me.untargetable = true
		}

		Event DestroyMe
		{
			NotCondition
			{
				Operator:	opHasTeammateNearby
				Param1:		"Turrets_PopUp_Turret"
				Param2:		3
			}

			Set: me.destroy = true
		}
	}
}
