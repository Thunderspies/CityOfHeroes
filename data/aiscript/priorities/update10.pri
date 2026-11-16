// EJ's CoV PL sandbox, please do not touch.


//**********************************************************************
// PL_UseAIConfig_Invincible - Normal combat, but entities won't die
//**********************************************************************

PriorityList PL_UseAIConfig_Invincible
{
	Priority Combat
	{
		set: me.Invincible = true
		Action: actionCombat
	}
}


//**********************************************************************
// PL_Follow_Route_Flying_No_Combat_Untargetable - Speaks for itself
//**********************************************************************

PriorityList PL_Follow_Route_Flying_No_Combat_Untargetable
{
	Priority Follow_Route
	{
		Set:		me.overridePerceptionRadius = 10
		Set:		me.Untargetable = true
		Set:		me.flying = true
		Action
		{
			Type:		actionFollowRoute
			doNotRun:	1
		}
	}
}
