#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TestTube1_Loop 35 35 .1
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName BeakerAnchor
			BhvrOverride
				PositionOffset 0 0 0
			End
			#Geom madscience_beaker_03
		End

	End

	Condition
		On Cycle
		Time 90

		Event
			At beakeranchor
			Type Local
			EName BeakerFX
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part WORLD\City\Science_Lab\Beaker_Steam.part
			LifeSpan 75
			LifeSpanJitter 15.0000
		End

		Event
			At beakeranchor
			Type Local
			EName BottomBurner
			BhvrOverride
				PositionOffset 0 -0.15 0
			End
			Part WORLD\City\Science_Lab\Beaker_BottomSmoke.part
			LifeSpan 15
			LifeSpanJitter 15.0000
		End

	End

End
