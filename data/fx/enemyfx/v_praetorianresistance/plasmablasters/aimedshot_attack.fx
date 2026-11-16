#############################################################
## RobotLazerBursts.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Shoot_Right
		Type	Local
		At	WepR
		Until	SHOOT RIGHT
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Snapshot_Left.fx
	End

	Event
		EName	Shoot_Left
		Type	Local
		At	WepL
		Until	SHOOT LEFT
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Snapshot_Right.fx
	End
End

Condition
	On Time
	Time 5
	
	Event
		Type	Local
		At	WepR
		sound	RES_Pistols_FireAimed_01 100.0 80.0 .9
		LifeSpan 5
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	28

	Event
		Ename	Prime
		Type	Start
		At	Root
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			TrackRate		10.0
		End
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	Prime
	End
End

#############################################################

End