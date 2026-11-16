FXinfo
Lifespan 400

##############################################################
Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	NA_Regrowth_01 100 100 .8
	End
End

Condition
	on time
	time 0

	Event
		At root
		type Local
		ChildFX :Regrowth_Activation.fx
	End
End

Condition
	On time
	Time 10

	Event
		At WepL
		Type Local
		Bhvr behaviors\GenericParticleFade.bhvr
		Part CustomizeablePowers\PlantControl\NatureAffinity\Hand_Glow.part
		Part CustomizeablePowers\PlantControl\NatureAffinity\Hand_Glow_Faint.part
		Lifespan 45
	End

	Event
		At root
		type Local
		ChildFX :Regrowth_Child.fx
	End
End


End