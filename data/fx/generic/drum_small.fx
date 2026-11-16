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
		Geom	Drum_small
#		Bhvr	Behaviors/GenericParticleFade.bhvr

	End


End

#Condition
#	On	Death
#
#	Event
#		Type	Local 
#		At	Root 
#		LifeSpan 1000
#	End
#End

End
##################################

