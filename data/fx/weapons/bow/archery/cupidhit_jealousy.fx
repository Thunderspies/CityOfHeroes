#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	CupidsBow_Hit_01 100 100 .35
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part WEAPONS\Bow\Archery\Heart_Dust_Down.part
			Part WEAPONS\Bow\Archery\Heart_Twinkle_Down.part
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part WEAPONS\Bow\Archery\Hearts_Broken.part
			LifeSpan 3
		End

	End

End
