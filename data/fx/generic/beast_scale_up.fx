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
		Bhvr	Behaviors/Beast_scale_UP.bhvr
	End


End


End
##################################

