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

LifeSpan 200


##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Straif_attack
		Type	Start
		At	Root
		Anim	Rikti_dropship
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

