#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End


##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Start
		At	Mystic 
		Geom	mystic_broken_turret
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Root 
		LifeSpan 360
	End
End

End
##################################

