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


##Flags InheritAlpha

##################################################################

###LifeSpan ??

Condition
	On	Time
	Time	50
	Event
		EName 	final_Natterling
		Type	Local
		At	Root
		Anim	FX_Natterling_crap
		Bhvr	Behaviors/quick_fadein.bhvr


	End


End

#######   Kill natterling  ##################


Condition
	On 	Time
	Time 	102
	
	Event
		EName 	final_Natterling
		Type	Destroy
		
	End


End


