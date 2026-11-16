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

Lifespan	21

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	Dice
		Type	Local
		At	BendMystic
		Geom	Prop_Coin
##		Bhvr	Behaviors/GenericParticleFade.bhvr

		End

End				