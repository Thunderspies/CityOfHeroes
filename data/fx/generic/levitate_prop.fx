#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End


##################################################################

Lifespan 90

Condition
	On	Time
	Time	0

	Event
		EName 	Prop_rise
		Type	Start
		At	Root 
		Anim	Prop_basic
	End

End

Condition
	On	Time
	Time	19

	Event
		EName	Prop_chunk
		Type	Local
		At	Prop_rise
		AnimPiv Hips
		geom	toss

	End


End



End
##################################

