#########################################################
##	
##

FxInfo

LifeSpan  107


Flags InheritAnimScale

##################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Anim	FX_RoboHatch
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
End

#####  Sound rise up ############################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound techdoor3 120 120 .46
	End

End





End				