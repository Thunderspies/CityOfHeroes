#                                                          
FxInfo

	Input
		InpName Chest
	End

	LifeSpan 40


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MM_ThrowSand_Hit_01 100 100 .9
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName geom
			Bhvr Behaviors\SpinHit2.bhvr
			Geom CenterDummy
		End

		Event
			At Chest
			Type Local
			EName hit2
			Part CustomizeablePowers\WaterBlast\Hit_Streaks.part
			Part CustomizeablePowers\WaterBlast\Splash_Flash.part
			Part CustomizeablePowers\MartialManipulation\Sand_Dust.part
			Part CustomizeablePowers\MartialManipulation\Sand_Spray.part
			LifeSpan 2
		End

	End

End
