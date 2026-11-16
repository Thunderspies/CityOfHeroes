#############################################
## Arachnos PTT Generator - Tanker Version ##
#############################################

FxInfo

########  Arachnos PTT Generator - Tanker Version  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_Orange			#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_StrobeOrange	#call to geometry fx
	End
End


End