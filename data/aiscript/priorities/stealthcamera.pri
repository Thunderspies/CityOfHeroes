

PriorityList PL_STEALTHCAMERA
{
	Priority Attack
	{
		Action: actionStandStill

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


PriorityList PL_ALARM
{
	Priority Attack
	{
		Action: actionStandStill

		Event Spawn
		{
			AtTime: 0
			Set:	me.flying = true
		}
	}
}



