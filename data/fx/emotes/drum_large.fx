#########################################################
## Emote Drum - Sound FX
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	origin
		Sound LowDrum 50 50 .7
	End
End

Condition
	On	Cycle
	Time	30

	Event
		Type	Local
		At	origin
		Sound LowDrum 50 50 .7
		Lifespan	29
	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	drum_stick_R
		Type	Local
		At	WepR 
		Geom	BoneR
	End

	Event
		EName 	drum_stick_L
		Type	Local
		At	WepL 
		Geom	BoneL
	End
End

#Condition
#	On	Time
#	Time	5
#
#	Event
#		EName 	drum
#		Type	Local
#		At	Mystic 
#		Geom	Drum_big
#	End
#End

#########################################################

End