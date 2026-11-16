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
	Time	5

	Event
		EName 	drum
		Type	Local  ##Start
		At	Mystic 
		Geom	Drum_big
#		Bhvr	Behaviors/GenericParticleFade.bhvr

	End


End

#Condition
#	On	Death
#
#	Event
#		Type	Local 
#		At	Root 
#		LifeSpan 900
#	End
#End

End
##################################

