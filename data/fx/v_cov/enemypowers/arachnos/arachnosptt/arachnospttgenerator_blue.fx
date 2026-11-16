############################
## Arachnos PTT Generator ##
############################

FxInfo

########  Arachnos PTT Generator - Tanker Version  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_Blue			#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_StrobeBlue	#call to geometry fx
	End
End


End