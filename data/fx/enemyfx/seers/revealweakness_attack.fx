#############################################################
## RevealWeakness_Attack.fx
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
		Sound Seers_RevealWeakness_Attack 100 100 1
	End
End

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	WristbandOffset
		Type	Local
		At	Head
		Bhvr	V_COV\Powers\ArachnosSoldier\PowerAnalyzerOffset_01.bhvr
	End

	Event
		EName	TargetChestOffset
		Type	PositOnly
		At	T_Chest
		LookAt	WristbandOffset
	End
End

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ScanningLaser
		Type	Local
		At	TargetChestOffset
		Bhvr	V_COV\Powers\ArachnosSoldier\Surveillance\ReticleScale1.bhvr
		part	:Beam_Core1.Part
		part	:Beam_Glow1.Part
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
		Bhvr	:ReticleScale.bhvr
		part	:Target_Reticle.part
		part	:Target_ReticleFill.part
		part	:Target_Scanner.part

	End
End

#############################################################

End