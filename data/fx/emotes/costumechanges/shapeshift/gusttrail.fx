#############################################################
## GustTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0
	Repeat 4

	Event
		Ename 	Spinner
		Type	PositOnly
		At	Chest
		BhvrOverride
			Spin 0.0 0.4 0.0
			SpinJitter 0.0 0.1 0.0
		End
	End


	Event
		Ename 	Spinner2
		Type	PositOnly
		At	Hips
		BhvrOverride
			Spin 0.0 -0.4 0.0
			SpinJitter 0.0 0.1 0.0
		End
	End



	Event
		Ename	Prime
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset 	8.0 5.0 12.0
			StartJitter	4.0 5.0	11.0
			TrackRate	.75
		End
		Part	:GustTrails.part
		Magnet 	Chest
		Lifespan 40
	End


	Event
		Ename	Prime
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 	-8.0 0.0 12.0
			StartJitter	4.0 5.0	11.0
			TrackRate	0.75
		End
		Part	:GustTrails.part
		Magnet 	Hips
		Lifespan 40
	End


End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	Offset
		Type	Destroy
	End
End


End