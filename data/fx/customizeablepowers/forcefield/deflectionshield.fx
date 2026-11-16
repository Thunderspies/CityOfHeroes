#############################################################
## DeflectionShield.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound ForceField5_loop 100.0 100.0 .31
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		Geom	RootToChestAdjustment
	End

	Event
		Type	Local
		At	Offset
		AltPiv	1
		Bhvr 	CustomizeablePowers\ForceField\ForceBubble2.bhvr
		Part	CustomizeablePowers\ForceField\bubblesphereSlim.part
		Part	CustomizeablePowers\ForceField\bubblesphereSlimMagnets2.part
		POther	Next
		PMagnet	Next
	End

	Event
		Type	Local
		At	Offset
		AltPiv	1
		Bhvr 	CustomizeablePowers\ForceField\ForceBubbleBright.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		LifeSpan 1
	End
End

#############################################################

End