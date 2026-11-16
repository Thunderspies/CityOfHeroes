#############################################################
## Header
#############################################################

FxInfo

Lifespan 50

## SOUNDS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound Training_Surveillance_Hit_01 120 120 .5
	End
End

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	WristbandOffset
		Type	Local
		At	T_LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
	End

	Event
		EName	TargetChestOffset
		Type	PositOnly
		At	Chest
		LookAt	WristbandOffset
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ScanningLaser
		Type	Local
		At	TargetChestOffset
		Bhvr	V_COV\Powers\ArachnosSoldier\Surveillance\ReticleScale1.bhvr
		part	V_COV\Powers\ArachnosSoldier\Surveillance\Beam_Core1.Part
		part	V_COV\Powers\ArachnosSoldier\Surveillance\Beam_Glow1.Part
		pOther	WristbandOffset
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	TargetReticle
		Type	Local
		At	TargetChestOffset
		Bhvr	V_COV\Powers\ArachnosSoldier\Surveillance\ReticleScale1.bhvr
		part	V_COV\Powers\ArachnosSoldier\Surveillance\Target_Reticle1.part
	End
End

#############################################################

End