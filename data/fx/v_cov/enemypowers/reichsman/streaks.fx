#############################################################
## GustTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	20
	Repeat	10

	Event
		Ename 	Spinner
		Type	PositOnly
		At	Root
		BhvrOverride
			Spin 0.0 0.2 0.0
			SpinJitter 0.0 0.1 0.0
			PositionOffset 0.0 10.0 0.0
    		End
		Lifespan 30
	End

	Event
		Ename	Prime
		Type	Local
		At	Spinner

		BhvrOverride
			PositionOffset 	0.0 4.0 0.0
			StartJitter	20.0 2.0 20.0
			TrackRate	.5
		End
		Magnet Root
		Part	:Streaks.part
		Lifespan 20
		LifespanJitter 10

	End

End

#############################################################


Condition
	On	Cycle
	Time	15
	Repeat	10


	Event
		Ename 	Spinner2
		Type	PositOnly
		At	Root
		BhvrOverride
			Spin 0.0 -0.2 0.0
			SpinJitter 0.0 0.1 0.0
			PositionOffset 0.0 10.0 0.0
		End
		Lifespan 10
	End

	Event
		Ename	Prime
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 	0.0 3.0 0.0
			StartJitter	15.0 1.0 15.0
			TrackRate	0.35
		End
		Magnet Root
		Part	:Streaks.part
		Lifespan 15
		LifespanJitter 5

	End

End