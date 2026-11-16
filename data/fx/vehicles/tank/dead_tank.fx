#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Origin
End

Input  
	Inpname	Hips
End

Input  
	Inpname	WepR
End

LifeSpan 350


##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Dead_tank
		Type	Start
		At	Hips
		Geom	Dead_tank
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
#	On	Death
#
#	Event
#		Type	Local 
#		At	Root 
#		LifeSpan 360
#	End
#End
#
#End
##################################

