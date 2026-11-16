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
			Spin 0.0 0.2 0.0
			SpinJitter 0.0 0.1 0.0
		End
	End


	Event
		Ename 	Spinner2
		Type	PositOnly
		At	Hips
		BhvrOverride
			Spin 0.0 -0.2 0.0
			SpinJitter 0.0 0.1 0.0
		End
	End



	Event
		Ename	Prime
		Type	Local
		At	Spinner
		BhvrOverride
		#	PositionOffset 	2.0 5.0 0.0
			StartJitter	4.0 2.0	20.0
			TrackRate	-1.0
		End
		Part	:GustTrailsOut.part
		Magnet 	Chest
		Lifespan 12
	End


	Event
		Ename	Prime
		Type	Local
		At	Spinner2
		BhvrOverride
		#	PositionOffset 	-2.0 0.0 0.0
			StartJitter	3.0 2.0	20.0
			TrackRate	-0.75
		End
		Part	:GustTrailsOut.part
		Magnet 	Hips
		Lifespan 13
	End


End

#############################################################


End