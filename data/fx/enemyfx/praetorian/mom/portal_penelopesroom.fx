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
			Part ENEMYFX\Praetorian\MoM\Rift_Hurricane_Des.part
			Part ENEMYFX\Praetorian\MoM\Rift_Hurricane_Swirl_Des.part
			Part ENEMYFX\Praetorian\MoM\Rift_Hurricane_Swirl_Highlights_Des.part
			Part ENEMYFX\Praetorian\MoM\Rift_Hurricane_Small_Des.part
			LifeSpan 10
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
			Part ENEMYFX\Praetorian\MoM\Rift_PenelopesRoom.part
			Part :Rift_BlackHole_NoiseScreen.part
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
			Part ENEMYFX\Praetorian\MoM\Rift_Flash_Des.part
			LifeSpan 0
		End

		Event
			At T_Root
			Type Start
			EName BlackHole
			BhvrOverride
				PositionOffset 0.0 10.0 0.0
			End
			Part :Rift_BlackHole_Expand.part
		End

	End

End
