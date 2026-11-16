#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	Chest
End


Flags InheritAlpha

##################################################################

LifeSpan 65

Condition
	On	Time
	Time	0
	Event
		EName 	2nd_Natterling
		Type	Local
		At	Root
		Anim	FX_Natterling_transform_spawn
		Bhvr	Behaviors/quick_fadein.bhvr


	End


End

#######   Kill natterling  ##################


Condition
	On 	Time
	Time 	13
	
	Event
		EName 	2nd_Natterling
		Type	Destroy
		
	End


End


