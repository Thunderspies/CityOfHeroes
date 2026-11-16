#############################################################
## SporeSleepBombContinuing.fx
#############################################################

FxInfo
Lifespan 60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	head
		Part	CustomizeablePowers\PlantControl\ChockingSporeDust.part
		Part	CustomizeablePowers\PlantControl\ChockingSporeDust2.part
		#Part	CustomizeablePowers\PlantControl\ChockingSporeGlows.part
		Lifespan  60
	End
End

#############################################################

End