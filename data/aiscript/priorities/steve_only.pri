
// This is Steve's test file, don't add mess with it.

//**********************************************************************
// PL_SteveTest
//**********************************************************************

PriorityList PL_SteveTest
{
	Priority Appear
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Raisedead", "Spawn"
		}

		Event DisablePriority
		{
			AtTime: 3
			Action: actionDisablePriority
		}
	}
	
	Priority Combat
	{
		Action: actionCombat
	}
}

