#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepR
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
		EName	Prop
		Type	Local
		At	WepR
		Geom	GEO_WepR_pickaxe
##		Bhvr	Behaviors/GenericParticleFade.bhvr

		End

End				