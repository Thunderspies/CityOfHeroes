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

##Lifespan	300

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	Dice
		Type	Start
		At	Mystic
		Geom	coinA__brightFX01
##		Bhvr	Behaviors/GenericParticleFade.bhvr

		End

End				