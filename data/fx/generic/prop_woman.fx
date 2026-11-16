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
		EName 	Woman
		Type	Local  ##Start
		At	Root 
		Anim	Prop_woman
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End



End
##################################

