############################
## Arachnos PTT Generator ##
############################

FxInfo

########  Arachnos PTT Generator - Power  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_Red		#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_StrobeRed	#call to geometry fx
	End
End


End