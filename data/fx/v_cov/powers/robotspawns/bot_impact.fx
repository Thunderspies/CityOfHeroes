FxInfo

LifeSpan	100

Condition
	On 	Time
	Time 	9

	Event
		EName	Ring01
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust.part
		LifeSpan	15		
	End

End

#####  Sound impact ############################

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	Root
		Sound BotImpact 120 120 .8
	End

End

#####  Sound open box ############################

Condition
	On 	Time
	Time 	32

	Event
		Type	Start
		At	Root
		Sound BigMetalHit2 120 120 .46
	End

End


#####  Sound robot unfold ############################

Condition
	On 	Time
	Time 	50

	Event
		Type	Start
		At	Root
		Sound techdoor3 120 120 .46
	End

End






End