#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepL
End

Input  
	InpName	Root
End

Input  
	InpName	head
End

##Lifespan	80

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	rock
		Type	Local
		At	Mystic
		Geom	RPS_rock__brightFX01
##		Bhvr	Behaviors/GenericParticleFade.bhvr

	
End

End				