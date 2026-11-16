#########################################################
##  Willpower - Continuing Effect - Heartbeat (Child)
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:WillPower_LightRays1.part
	End
End

#########################################################

End			