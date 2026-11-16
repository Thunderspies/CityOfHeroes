
//**********************************************************************
// PL_TentacleHiding
//**********************************************************************

PriorityList PL_TentacleHiding
{
	Priority StayHidden
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Spawn"
		}
	}
}

//**********************************************************************
// PL_TentacleRanged
//**********************************************************************

PriorityList PL_TentacleRanged
{
	Priority Spawn
	{
		Action: actionFrozenInPlace

		Event Done
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}

	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}


//**********************************************************************
// PL_TentacleStillRanged
//**********************************************************************

PriorityList PL_TentacleStillRanged
{
	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}

//**********************************************************************
// PL_TentacleMelee
//**********************************************************************

PriorityList PL_TentacleMelee
{
	Priority Spawn
	{
		Action: actionFrozenInPlace

		Event Done
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}

	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}

//**********************************************************************
// PL_TentacleStillMelee
//**********************************************************************

PriorityList PL_TentacleStillMelee
{
	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}

//**********************************************************************
// PL_TentacleSpawned
//**********************************************************************

PriorityList PL_TentacleSpawned
{
	Priority StayHidden
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Spawn"
		}

		Event Done
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority Spawn
	{
		Action: actionFrozenInPlace

		Event Done
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}

	Priority Snipe
	{
		Set: me.doNotMove = true

		Action: actionCombat
	}
}
