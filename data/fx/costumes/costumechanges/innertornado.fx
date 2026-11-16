#############################################################
## GustTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time 	8
	Repeat 	2
	Event
		Ename 	Spinner
		Type	PositOnly
		At	Head
		BhvrOverride
			Spin 0.0 .75 0.0
			SpinJitter 0.0 0.15 0.0
		End
		Lifespan 40
	End




	Event
		Ename	Prime
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset 	1.0 .5 5.0
			StartJitter	1.0 0.5	1.0
			Gravity		.015
			TrackRate	.4
		End
		Part	:InnerGustTrailsAdditive.part

		Magnet 	FootL
		Lifespan 40
	End

End

Condition
	On	Cycle
	Time 	4


	Event
		Ename 	Spinner2
		Type	PositOnly
		At	Head
		BhvrOverride
			Spin 0.0 .75 0.0
			SpinJitter 0.0 0.15 0.0
		End
		Lifespan 40
	End

	Event
		Ename	Prime
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 	1.0 1.0 5.0
			StartJitter	1.0 0.5	1.0
			Gravity		.015
			TrackRate	.35
		End
		Part	:InnerGustTrails.part

		Magnet 	FootL
		Lifespan 40
	End
End

#	Event
#		Ename 	Spinner2
#		Type	PositOnly
#		At	Hips
#		BhvrOverride
#			Spin 0.0 -0.4 0.0
#			SpinJitter 0.0 0.1 0.0
#		End
#	End

#	Event
#		Ename	Prime
#		Type	Local
#		At	Spinner2
#		BhvrOverride
#			PositionOffset 	-10.0 4.0 0.0
#			StartJitter	3.0 2.0	20.0
#			TrackRate	0.75
#		End
#		Part	:GustTrails.part
#		Magnet 	Hips
#	#	Lifespan 60
#	End


End

#############################################################

#Condition
#	On	PrimeHit
#
#	Event
#		EName	Prime
#		Type	Destroy
#	End
#
#	Event
#		EName	Offset
#		Type	Destroy
#	End
#End

#	Event
#		Ename	Aimer
#		Type	Local
#		At	Offset
#		Magnet	ChestOffset
#		BhvrOverride
#			TrackRate 5.0
#			TrackMethod 2
#		End
#		Part	:GustHeads.part
#	End


#	Event
#		Ename 	Offset
#		Type 	Local
#		At	ChestMagnet
#		#BhvrOverride
#		#	PositionOffset 	0.0 3.0 10
#		#	StartJitter	0.0 4.0 3.0
#		#End
#		Part	Costumes\CostumeChanges\GustTrails.part
#	#	Lifespan 30
#	End





#############################################################

