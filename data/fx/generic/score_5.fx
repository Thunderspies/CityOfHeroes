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

##Lifespan	150

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	Score
		Type	Local
		At	WepL
		Geom	Prop_Score_5
		Bhvr	Behaviors/fadein.bhvr

	
End

End				