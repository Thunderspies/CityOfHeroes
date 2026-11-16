#########################################################
##	
##

FxInfo


Input  
	InpName	BendMystic
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

Lifespan	30

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	Dice
		Type	Local
		At	BendMystic
		Geom	Prop_dice2
##		Bhvr	Behaviors/GenericParticleFade.bhvr

		End

End				