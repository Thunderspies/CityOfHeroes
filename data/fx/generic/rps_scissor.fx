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
		EName	scissors
		Type	Local
		At	Mystic
		Geom	RPS_scissor__brightFX01
##		Bhvr	Behaviors/GenericParticleFade.bhvr

	
End

End				