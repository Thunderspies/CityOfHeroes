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
		EName 	Prime
		Type	Local 
		At	Root 
		Geom	plane
		Bhvr	Behaviors/GenericParticleFade.bhvr

	//	Lifespan 100
	End


End

End
##################################

