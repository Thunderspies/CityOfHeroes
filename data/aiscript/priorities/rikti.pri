
//**********************************************************************
// PL_RiktiShip
//**********************************************************************

PriorityList PL_RiktiShip
{
	Priority Enter
	{		
		Action
		{
			// Ship flies in, stay in place until bit is turned off.

			Type:		actionFrozenInPlace
			AnimBits:	"spawn"
		}

		Event DoneFlyingIn
		{
			AtTime: 60
			Action: actionDisablePriority
		}
	}

	Priority BeamDownTroops
	{		
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"spawn"
		}

		Event BeamGuysIn
		{
			AtTime: 0

			Action
			{
				Type:	actionPlayFX
				FXFile:	"GENERIC\SpaceShipSpawnFX.fx"
			}
		}
	}
}

//**********************************************************************
// PL_BetaEvent_RiktiFlyby
//**********************************************************************

PriorityList PL_BetaEvent_RiktiFlyby
{
	Priority Flyby
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"FLY", "FAR"
		}
		Event DestroyMe
		{
			// Destroy the entity after 33 seconds.

			AtTime: 65

			Set: me.destroy = true
		}
	}
}


//**********************************************************************
// PL_BetaEvent_RiktiAttack
//**********************************************************************

PriorityList PL_BetaEvent_RiktiAttack
{
	Priority FlyIn
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"FLY", "MID"
		}

		Event Done
		{
			AtTime:		33
			Action: actionDisablePriority
		}
	}

	Priority BeamIn
	{
		Action
		{
			Type:		actionFrozenInPlace
		}

		Event BeamGuysIn
		{
			AtTime: 0

			Action
			{
				Type:	actionPlayFX
				FXFile:	"GENERIC\SpaceShipSpawnFX.fx"
			}
		}
	}

}

//**********************************************************************
// PL_RiktiShipLeave
//**********************************************************************

PriorityList PL_RiktiShipLeave
{
	Priority Wait
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"spawn"
		}
		
		Event DisablePriority
		{
			AtTime:	5
			Action: actionDisablePriority
		}
	}
	
	Priority Leave
	{		
		// Anim bits turned off.  This will fly ship away.

		Action: actionFrozenInPlace

		Event DestroyMe
		{
			// Destroy the entity after 33 seconds.

			AtTime: 33

			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_RiktiTroop
//**********************************************************************

PriorityList PL_RiktiTroop
{
	Priority WaitForShipToLand
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DisableFindTarget
		{
			AtTime:	0
			Set:	me.findTarget = false
			Set:	list.hold = true
		}

		Event DisablePriority
		{
			// Change this to 60 to have the correct timing.
			AtTime:	0
			Action:	actionDisablePriority
		}
	}

	Priority WaitForAWhile
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DoneWaiting
		{
			AtTime:		1
			//Condition:	random.percent < 10
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		2
			Condition:	random.percent < 11
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		3
			Condition:	random.percent < 13
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		4
			Condition:	random.percent < 15
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		5
			Condition:	random.percent < 17
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		6
			Condition:	random.percent < 20
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		7
			Condition:	random.percent < 25
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		8
			Condition:	random.percent < 33
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		9
			Condition:	random.percent < 50
			Action:		actionDisablePriority
		}

		Event Appear
		{
			AtTime:	10
			Action: actionDisablePriority
		}
	}
	
	Priority BeamIn
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}

		Event PlayFX_StayInvisible
		{
			AtTime:		0

			Set: me.doNotDrawOnClient = true

			Action {
				Type:	actionPlayFX
				FXFile:	"ENEMYFX\rikti\riktiportal.fx"
			}
		}
		
		Event PickWeapon_Appear
		{
			AtTime:	2
			Action: actionUseRandomPower
			Action: actionDisablePriority
		}
	}

	Priority WaitForFXToFinish
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}
		
		Event DisablePriority
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}
	
	Priority WaitForWeaponPullToFinish
	{
		Action
		{
			Type: actionFrozenInPlace
		}
		
		Event DisablePriority
		{
			AtTime: 2.5
			Action: actionDisablePriority
		}
	}
	
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_RiktiTroopAttack
//**********************************************************************

PriorityList PL_RiktiTroopAttack
{
	Priority WaitForShipToLand
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DisableFindTarget
		{
			AtTime:	0
			Set:	me.findTarget = false
			Set:	list.hold = true
		}

		Event DisablePriority
		{
			// Change this to 60 to have the correct timing.
			AtTime:	60
			Action:	actionDisablePriority
		}
	}

	Priority WaitForAWhile
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DoneWaiting
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		2
			Condition:	random.percent < 11
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		3
			Condition:	random.percent < 13
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		4
			Condition:	random.percent < 15
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		5
			Condition:	random.percent < 17
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		6
			Condition:	random.percent < 20
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		7
			Condition:	random.percent < 25
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		8
			Condition:	random.percent < 33
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		9
			Condition:	random.percent < 50
			Action:		actionDisablePriority
		}

		Event Appear
		{
			AtTime:	10
			Action: actionDisablePriority
		}
	}
	
	Priority BeamIn
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}

		Event PlayFX_StayInvisible
		{
			AtTime:		0

			Set: me.doNotDrawOnClient = true

			Action {
				Type:	actionPlayFX
				FXFile:	"ENEMYFX\rikti\riktiportal.fx"
			}
		}
		
		Event PickWeapon_Appear
		{
			AtTime:	2
			Action: actionUseRandomPower
			Action: actionDisablePriority
		}
	}

	Priority WaitForFXToFinish
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}
		
		Event DisablePriority
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}
	
	Priority WaitForWeaponPullToFinish
	{
		Action
		{
			Type: actionFrozenInPlace
		}
		
		Event DisablePriority
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}
	
	Priority Combat
	{
		Action: actionCombat
	}
}

//**********************************************************************
// PL_RiktiTroopWander
//**********************************************************************

PriorityList PL_RiktiTroopWander
{
	Priority WaitForShipToLand
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DisableFindTarget
		{
			AtTime:	0
			Set:	me.findTarget = false
			Set:	list.hold = true
		}

		Event DisablePriority
		{
			// Change this to 60 to have the correct timing.
			AtTime:	60
			Action:	actionDisablePriority
		}
	}

	Priority WaitForAWhile
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event DoneWaiting
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		2
			Condition:	random.percent < 11
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		3
			Condition:	random.percent < 13
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		4
			Condition:	random.percent < 15
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		5
			Condition:	random.percent < 17
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		6
			Condition:	random.percent < 20
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		7
			Condition:	random.percent < 25
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		8
			Condition:	random.percent < 33
			Action:		actionDisablePriority
		}

		Event DoneWaiting
		{
			AtTime:		9
			Condition:	random.percent < 50
			Action:		actionDisablePriority
		}

		Event Appear
		{
			AtTime:	10
			Action: actionDisablePriority
		}
	}
	
	Priority BeamIn
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}

		Event PlayFX_StayInvisible
		{
			AtTime:		0

			Set: me.doNotDrawOnClient = true

			Action {
				Type:	actionPlayFX
				FXFile:	"ENEMYFX\rikti\riktiportal.fx"
			}
		}
		
		Event PickWeapon_Appear
		{
			AtTime:	2
			Action: actionUseRandomPower
			Action: actionDisablePriority
		}
	}

	Priority WaitForFXToFinish
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	spawn
		}
		
		Event DisablePriority
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}
	
	Priority WaitForWeaponPullToFinish
	{
		Action
		{
			Type: actionFrozenInPlace
		}
		
		Event DisablePriority
		{
			AtTime: 2
			Action: actionDisablePriority
		}
	}
	
	Priority Wander
	{
		Condition: me.hasAttackTarget == false

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			Radius:		100
		}
	}

	Priority Combat
	{
		Action: actionCombat
	}
}


//**********************************************************************
// PL_RMKF_Start
//**********************************************************************
PriorityList PL_RMKF_Start
{
	Priority BeamIn
	{
		Action: actionFrozenInPlace
	}
}

//**********************************************************************
// PL_RMKF_Fight
//**********************************************************************
PriorityList PL_RMKF_Fight
{
	Priority BeamIn
	{
		Set: me.invisible = true
		Action: actionFrozenInPlace

		Event BeamIn
		{
			AtTime:	0
			Action
			{
				Type:	actionPlayFX
				FXFile:	"ENEMYFX\rikti\riktiportalstart.fx"
			}
		}
		Event Next
		{
			AtTime:	3
			Set: me.invisible = false
			Action: actionDisablePriority
		}
	}
	Priority Combat
	{
		Action: actionCombat
	}
}


//**********************************************************************
// PL_RMKF_Complete
//**********************************************************************
PriorityList PL_RMKF_Complete
{
	Priority Celebrate
	{
		Action: actionCombat

		Event Celebrate
		{
			AtTime: 0
			Action: actionWander
		}
		Event DoneCelebrating
		{
			// Destroy the entity after 60 seconds.
			AtTime: 5
			Action: actionDisablePriority
		}
	}
	Priority Beam
	{
		Action: actionFrozenInPlace
		Event Beam
		{
			// Destroy the entity after 60 seconds.
			AtTime: 1
			Action 
			{
				Type:	actionPlayFX
				FXFile:	"ENEMYFX\rikti\riktiportal.fx"
			}
		}
		Event Destroy
		{
			AtTime: 7
			Set: me.destroy = true
			Action: actionDisablePriority
		}
	}
}

