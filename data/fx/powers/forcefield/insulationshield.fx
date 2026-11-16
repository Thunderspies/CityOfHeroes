#############################################################
## InsulationShield.fx
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
		Sound ForceField3_loop 100.0 100.0 0.42
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
		Type	Posit
		At	HIPS
		Bhvr 	Powers\ForceField\ForceBubble2.bhvr
		Part1	Powers\ForceField\bubblesphere3.part
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