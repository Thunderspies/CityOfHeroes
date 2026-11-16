#########################################################
##	
##

FxInfo

LifeSpan  200


Flags InheritAnimScale

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Anim	FX_RobotBox
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
End

End				