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
	Time	10
	Event
		EName 	prop
		Type	start
		At	Root 
		Geom	scarecrow_pole
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Root 
		LifeSpan 1999
	End
End

End
##################################

