#############################################################
## GustTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Ename	ConeOffset
		Type	Start
		At	Root
		BhvrOverride
			PYRRotate	0 180 0
			Scale		10 10 10
		End
		GEOM	FX_ConeOffsets
	End

	Event
		EName 	TargetOffset
		Type	Local
		At	ConeOffset
		AltPiv	1
	End

	Event
		Ename 	Spinner
		Type	Start
		At	Hips
		BhvrOverride
			TrackRate	2.75
		End
		Magnet	Target
		Lookat	Target
	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Spinner
		BhvrOverride
			Spin 			0.0 -.5 0.0
		End
	End
End

Condition
	On	Time
	Time	10
	Repeat	2

	Event
		Type	Start
		At	TargetOffset
		BhvrOverride
			Trackrate		3.0
			InitialVelocity		0.0 0.75 0
			InitialVelocityJitter	1.0 0.5 1.0
			Gravity			0.04
			PositionOffset		0.0 2.0 0
			StartJitter		2.0 3.0 2.0
		End
		Magnet	Spinner2
		Lookat	Spinner2
		Part	:GustTrails_Big.part
	End







End

#############################################################

End