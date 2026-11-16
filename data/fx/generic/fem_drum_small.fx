#########################################################
##	
########################################################
FxInfo


##################################################################

Condition
	On	Time
	Time	5

	Event
		EName 	drum
		Type	Local  ##Start
		At	Mystic
		Geom	Drum_small
			BhvrOverride
                                    Behavior
                                    PositionOffset		.5 .2 0
			End

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

