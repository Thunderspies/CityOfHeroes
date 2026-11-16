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
		Geom	Prop_box_small
		Bhvr	Behaviors/GenericParticleFade.bhvr

//		Lifespan 400
	End


End

End
##################################

