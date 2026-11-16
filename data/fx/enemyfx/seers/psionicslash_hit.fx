#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################


Condition
	On	Time
	Time	0

	Event
		Ename	ConeOffset
		Type	Local
		At	head
		Sound Seers_PsionicSlash_Hit 50 50 1
		BhvrOverride
			Scale		2 2 2
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName	TargetOffset
		Type	Local
		At	ConeOffset
		AltPiv	1
	End

	Event
		Type	Start
		At	TargetOffset
		BhvrOverride
			Scale		0.75 0.75 0.75
			PyrRotate	90 0 0
		End
		Part	:Head_Scratches.part
		Lifespan	10
	End

	Event
		Type	Start
		At	Head
		Part	:Head_Glow_Growing.part
		Part	:Head_Shockwave.part
		Part	:Head_Rings_Growing.part

		Lifespan	10
	End

End


##############################################################################

End