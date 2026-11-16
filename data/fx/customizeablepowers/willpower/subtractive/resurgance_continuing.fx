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
		Part	:Resurgance_Continuing_Rays1.part
	End
End

#########################################################

Condition
	On	Cycle
	Time	60
	Chance	0.125
	
	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:Resurgance_Continuing_Pulse.fx
		Lifespan	60
	End
End

#########################################################

End