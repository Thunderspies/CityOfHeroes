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
			Part WEAPONS\Bow\Archery\Hearts.part
			Part WEAPONS\Bow\Archery\Heart_Twinkle.part
			Part WEAPONS\Bow\Archery\Heart_TwinkleClump.part
			Part WEAPONS\Bow\Archery\Heart_Dust.part
		End

	End

End
