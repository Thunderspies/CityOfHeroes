############################
## Arachnos PTT Generator ##
############################

FxInfo

########  Arachnos PTT Generator - Healing  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_Green		#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_StrobeGreen	#call to geometry fx
	End
End


End