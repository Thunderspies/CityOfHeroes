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
		EName 	Bucket
		Type	Start
		At	Root 
		Geom	drill_mockup
#                        BhvrOverride
#                                    Behavior
#                                    PyrRotate		0 90 0
#			End

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

