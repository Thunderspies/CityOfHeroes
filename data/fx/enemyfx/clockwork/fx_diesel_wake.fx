#########################################################
##	tenticle fun yea!
########################################################
FxInfo

Input  
	Inpname	Root
End

Lifespan	500
##################################################################

Condition
	On	Time
	Time	0


	Event
		EName 	Scaffolding
		Type	Start 
		At	Root 
		Anim	FX_Diesel_wake
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End




End


