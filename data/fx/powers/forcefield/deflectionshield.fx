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
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\ForceBubble2.bhvr
		Part	Powers\ForceField\bubblesphereSlim.part
		Part	Powers\ForceField\bubblesphereSlimMagnets2.part
		POther	Next
		PMagnet	Next
	End

	Event
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\ForceBubbleBright.bhvr
		Geom	FX_ForceBubble
		LifeSpan 1
	End
End

#############################################################

End