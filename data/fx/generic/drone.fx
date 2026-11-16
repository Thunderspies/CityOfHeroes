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
		EName 	Drone_critter
		Type	Local
		At	Root 
		Anim	Nemesis_Drone
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End



End
##################################

