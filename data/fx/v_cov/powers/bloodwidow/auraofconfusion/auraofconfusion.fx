#############################################################
## Blood Widow "Aura of Confusion" FX
#############################################################

FxInfo

LifeSpan 150

## SOUND FX ###########################################################

Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound WorldOfConfusion_Start_01 80 80 .8
	End
End

Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Bhvr	behaviors\soundFade5.bhvr
		Sound WorldOfConfusion_Loop 100.0 100.0 .5
		Flags	PowerLoopingSound
		Lifespan 120
	End
End

## HANDS ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Geom	centerdummy
	End

	Event
		Type	local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	V_COV\Powers\Psionics\EnergyHandGlows.part
		LifeSpan 30
	End

	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	V_COV\Powers\Psionics\ringSpin.bhvr
		Part	V_COV\Powers\Psionics\SpinningHandRings.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		Type	PositOnly
		At	HIPS
		Bhvr 	:AuraOfConfusion.bhvr
		BhvrOverride
			Spin	0.0 0.01 0.0
		End
		Part1	POWERS\Psionics\WorldOfConfusion_Motes1.part
		Part1	POWERS\Psionics\WorldOfConfusion_Motes2.part
		Part1	:AuraOfConfusion_BubbleEdge1.part
		Geom	FX_PillBoxForceField #FX_ForceBubble
		Lifespan 15
	End

	Event
		Type	PositOnly
		At	HIPS
		Bhvr 	:AuraOfConfusion.bhvr
		BhvrOverride
			Spin	0.0 -0.01 0.0
		End
		Part1	POWERS\Psionics\WorldOfConfusion_Motes3.part
		Part1	POWERS\Psionics\WorldOfConfusion_Motes4.part
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Hips
		Part	:Ring1.part
		Part	:Ring2.part
		LifeSpan 20
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	20

	Event
		Ename	ExplosiveForce
		Type	Posit
		At	Hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan 30
	End
End

#############################################################

End