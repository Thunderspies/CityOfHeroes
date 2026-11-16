#
FxInfo


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Portal_loop 100 100 .5
	End
End

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type Start
			EName Windup
			BhvrOverride
				PositionOffset 0 10 0
			End
			Part :Rift_Hurricane.part
			Part :Rift_Hurricane_Small.part
			Part :Rift_Hurricane_Swirl.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 35

		Event
			At T_Root
			Type Start
			EName Flash
			BhvrOverride
				PositionOffset 0.0 10.0 0.0
			End
			Part ENEMYFX\Praetorian\MoM\Rift_Flash.part
			LifeSpan 0
		End

		Event
			At T_Root
			Type Start
			EName BlackHole
			BhvrOverride
				PositionOffset 0.0 10.0 0.0
			End
			Part ENEMYFX\Praetorian\MoM\Rift_BlackHole_Expand_Yellow.part
		End

	End

	Condition
		On Time
		Time 45

		Event
			At T_Root
			Type Start
			EName NoiseScreen
			BhvrOverride
				PositionOffset 0.0 10.0 0.0
			End
			Part ENEMYFX\Praetorian\MoM\Rift_AurorasMind.part
			Part :Rift_BlackHole_NoiseScreen.part
		End

	End

End
