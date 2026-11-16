#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Head
End


Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End


##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Tree_hair
		Type	Local
		At	Head
		Geom	TreeHair__treeleaves
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


	Event
		EName 	Tree_handR
		Type	Local
		At	WepR
		Geom	TreeHair__treeleaves
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


	Event
		EName 	Tree_handL
		Type	Local
		At	WepL
		Geom	TreeHair__treeleaves
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Head 
		LifeSpan 60
	End
End


End
##################################

