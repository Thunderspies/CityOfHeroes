#############################################################
## FX_birdwings.fx
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Anim	FX_birdwings
		Sound Seagull2 70 70 0.6
	End
End

#############################################################

End