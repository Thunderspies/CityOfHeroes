#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End


##################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	drum
		Type	Start
		At	Root 
		Geom	sprocket_junk_A
		Geom	sprocket_junk_B
		Geom	sprocket_junk_kneel_A
		Bhvr	Behaviors/GenericParticleFade.bhvr
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

