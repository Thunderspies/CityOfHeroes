#############################################################
## GustTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	6

	Event
		Ename 	Spinner
		Type	StartPositOnly
		At	Head
		BhvrOverride
			Spin 0.0 .5 0.0
			SpinJitter 0.0 0.15 0.0
		End
		Lifespan 30
	End
End

Condition
	On	Time
	Time 	7


	Event
		Ename 	Spinner2
		Type	StartPositOnly
		At	Root
		BhvrOverride
			Spin 0.0 -.4 0.0
			SpinJitter 0.0 0.15 0.0
		End
		Lifespan 35
	End

	Event
		Ename	Prime
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 	1.0 -0.2 1.5
			StartJitter	2.0 0.5	0.0
			Gravity		-.02
			TrackRate	-0.2
		End
		Part	:GustTrails.part

		Magnet 	WepL
		Lifespan 25
	End
End


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

