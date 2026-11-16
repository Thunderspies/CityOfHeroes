
// This is Steve's test file, don't add mess with it.

//**********************************************************************
// PL_SteveTest
//**********************************************************************

PriorityList PL_SteveTest2
{
	Priority Appear
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	RumbleA
		}

		Event DisablePriority
		{
			AtTime: 12
			Action: actionDisablePriority
		}
	}
	
	Priority Combat
	{
		Action: actionCombat
	}
}


PriorityList PL_AAListener
{
	Priority Listen
	{
		IReactTo:	Preacher
	}
}

PriorityList PL_AAPreacher
{
	Priority Listen
	{
		IAmA:	Preacher
		Action
		{
			Type:		actionDoAnim
			AnimBits:	HIGH ENCOUNTER
		}
	}
}

