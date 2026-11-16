#########################################################
## Emote DrumLow - Sound FX
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	origin
		Sound HiDrum_Loop 50 50 .6
	End
End

#########################################################

Condition
	On	Time
	Time	5

	Event
		EName 	SmallDrum
		Type	Local
		At	Mystic
		BhvrOverride
			PositionOffset	0.5 0.2 0
		End
		Geom	Drum_small
	End
End

#########################################################

End