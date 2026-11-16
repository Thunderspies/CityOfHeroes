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
		Anim	def
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End


End
##################################

