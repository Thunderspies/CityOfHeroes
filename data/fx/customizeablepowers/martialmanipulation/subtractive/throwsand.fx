#############################################################
## SweepingStrike_Fast.fx
#############################################################

FxInfo
Lifespan 100
#########################################################

Condition
	On	Time
	Time	15

	Event
		At 	Root
		Type 	Local
		EName 	AirRush1
		Bhvr 	behaviors\Braz\ExpandRingFast.bhvr
		BhvrOverride
			Scale 0.05 0.05 0.05
		End
		ChildFx CustomizeablePowers\MartialManipulation\Subtractive\ThrowSand_Child.fx
	End

End

Condition
	On Time
	Time 17

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset -2 0 1
			PyrRotate 0 -8 0
		End
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Dust_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Core_Traveling.part
		Lifespan 3
	End

End

Condition
	On Time
	Time 15

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset 0 0 3
			PyrRotate 0 0 0
		End
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Dust_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Core_Traveling.part
		Lifespan 3
	End

End

Condition
		On Time
		Time 13

	Event
			At Root
			Type Local
			EName AirRushAnchor
			BhvrOverride
				PositionOffset 2 0 0
				PyrRotate 0 8 0
			End
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Dust_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Traveling.part
		Part	CustomizeablePowers\MartialManipulation\Subtractive\Sand_Grains_Core_Traveling.part
		Lifespan 3
	End

End

End
