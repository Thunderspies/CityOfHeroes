#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition

	Event
		EName	JuggleRig
		Type	Local
		At	Mystic
		Anim	FX_JuggleBalls_Female
	End
End

## BALLS ###########################################################

Condition
	On 	Time
	Time 	12

	Event
		EName 	Ball1
		Type	Local
		At	JuggleRig
		AnimPiv	Neck
		BhvrOverride
			Scale			0.75 0.75 0.75
		End
		Geom	GEO_GenericBall_Red
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Ball2
		Type	Local
		At	JuggleRig
		AnimPiv	Waist
		BhvrOverride
			Scale			0.75 0.75 0.75
		End
		Geom	GEO_GenericBall_Yellow
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Ball3
		Type	Local
		At	JuggleRig
		AnimPiv	Chest
		BhvrOverride
			Scale			0.75 0.75 0.75
		End
		Geom	GEO_GenericBall_Blue
	End
End

#############################################################

End