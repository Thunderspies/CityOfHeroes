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
		Geom	GEO_ArachnosPTT			#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Chest
		Geom	GEO_ArachnosPTT_StrobePurple	#call to geometry fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Part1
		Type	Local
		At	Hips
		Geom	GEO_ArachnosPTTBase		#call to geometry fx
	End
End

End