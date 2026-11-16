#
FxInfo


###########################################################

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	Root
		Sound 	MagiCarpet_Start_01 100 100 .5
		Sound 	MagiCarpet_Start_02 100 100 .5
		Sound 	MagiCarpet_Start_03 100 100 .5
	End

	Event
		Type	Local
		At	Root
		Sound 	FlightFlyNew_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
	End

	Event
		Type	Local
		At	Root
		Sound 	FlightWind_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn5Out5.bhvr
	End

	Event
		EName 	StreakAnchor
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 -0.2 0.5
		End
		Part :streak.part
	End

	Event
		Ename	DustAnchor1
		Type	Local
		At	StreakAnchor
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.75 0 0
		End
	End

	Event
		Ename	DustAnchor2
		Type	Local
		At	StreakAnchor
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -1.75 0 0
		End
		Pother DustAnchor1
		Part :PixieDustClump.part
		Part :pixiedust.part
		Part :pixiedust2.part
	End

End

###########################################################
End
