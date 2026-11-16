
//**********************************************************************
// PL_ThankHero: Run up to hero who "saved" me.
//**********************************************************************

PriorityList PL_ThankHero
{
	Priority DoEverything
	{
		Action: actionNPC.ThankHero
	}
}

//**********************************************************************
// PL_MissionScared: Run around being scared.
//**********************************************************************

PriorityList PL_MissionScared
{
	Priority DoEverything
	{
		Action: actionNPC.Hostage
	}
}

//**********************************************************************
// PL_BeAttackable
//**********************************************************************

PriorityList PL_BeAttackable
{
	Priority BeAttackable
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0

			Set: me.findTarget = false
			Set: me.invincible = true
			Set: me.alwaysGetHit = true
			Set: me.doNotDrawOnClient = true
			Set: me.onTeamGood = true
		}
	}
}



//**********************************************************************
// PL_BeGoodAndFight
//**********************************************************************

PriorityList PL_BeGoodAndFight
{
	Priority BeGoodAndFight
	{
		Action: actionCombat

		Event Next
		{
			AtTime: 0
			Set: me.onTeamGood = true
		}
	}
}

//**********************************************************************
// PL_BeGoodAndStill
//**********************************************************************

PriorityList PL_BeGoodAndStill
{
	Priority BeGoodAndStill
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0
			Set: me.findTarget = false
			Set: me.onTeamGood = true
		}
	}
}

//**********************************************************************
// PL_BeStill
//**********************************************************************

PriorityList PL_BeStill
{
	Priority BeStill
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0
			Set: me.findTarget = false
		}
	}
}

//**********************************************************************
// PL_BeStillAndUntargetable
//**********************************************************************

PriorityList PL_BeStillAndUntargetable
{
	Priority BeStillAndUntargetable
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0
			Set: me.findTarget = false
			Set: me.untargetable = true
		}
	}
}

//**********************************************************************
// PL_BeEvilAndFight
//**********************************************************************

PriorityList PL_BeEvilAndFight
{
	Priority BeEvilAndFight
	{
		Action: actionCombat

		Event Next
		{
			AtTime: 0
			Set: me.onTeamEvil = true
		}
	}
}

//**********************************************************************
// PL_BeEvilAndStill
//**********************************************************************

PriorityList PL_BeEvilAndStill
{
	Priority BeEvilAndStill
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0
			Set: me.findTarget = false
			Set: me.onTeamEvil = true
			Set: me.untargetable = false
		}
	}
}

//**********************************************************************
// PL_BeGoodAndStillAndUntargetable
//**********************************************************************

PriorityList PL_BeGoodAndStillAndUntargetable
{
	Priority PL_BeGoodAndStillAndUntargetable
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0
			Set: me.untargetable = true
			Set: me.findTarget = false
			Set: me.onTeamGood = true
		}
	}
}

//**********************************************************************
// PL_BeNothingAT
//**********************************************************************

PriorityList PL_BeAnUnkillableNothing
{
	Priority BeAttackable
	{
		Action: actionFrozenInPlace

		Event Next
		{
			AtTime: 0

			Set: me.untargetable = true
			Set: me.findTarget = false
			Set: me.findTarget = false
			Set: me.invincible = true
			Set: me.doNotDrawOnClient = true
		}
	}
}
