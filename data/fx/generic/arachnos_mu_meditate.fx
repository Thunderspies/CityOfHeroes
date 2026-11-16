#########################################################
##	FireRing
##
FxInfo

#LifeSpan	70	

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	root
End

Input  
	Inpname	head
End



##########################################################
Condition
	On 	Time
	Time 	0


	Event
		EName 	glow
		Type	Local 
		At	Chest
		childFX	GENERIC/mu_med_bodyglow.fx
				
	End

End 

Condition
	On 	Time
	Time 	0


	Event
		EName 	OrbStreak
		Type	Local 
		At	Chest
		bhvr	behaviors/mu_Spin.bhvr
				
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv 0
		
		part3	:mumed.part
			
		
	End

End

	


	Condition
	On 	Time
	Time 	0


	Event
		EName 	glow
		Type	Local 
		At	root
		childFX	GENERIC/mu_med_sparks.fx				
	End

End 

Condition
	On 	Time
	Time 	3


	Event
		EName 	glow
		Type	Local 
		At	root
		childFX	GENERIC/mu_med_sparks.fx
				
	End

End 

Condition
	On 	Time
	Time 	3


	Event
		EName 	glow
		Type	Local 
		At	root
		childFX	GENERIC/mu_med_sparks.fx
				
	End

End 

Condition
	On 	Time
	Time 	3


	Event
		EName 	glow
		Type	Local 
		At	root
		childFX	GENERIC/mu_med_sparks.fx
				
	End

End 

Condition
	On 	Time
	Time 	3


	Event
		EName 	glow
		Type	Local 
		At	root
		childFX	GENERIC/mu_med_sparks.fx
				
	End

End 