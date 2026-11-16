#############################################################
## FX_DoveWings.fx
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
		Anim	FX_DoveWings
		#Sound	Seagull2 70 65 0.6
	End
End

#############################################################

End