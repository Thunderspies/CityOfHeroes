#
FxInfo

	LifeSpan 100


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SM_PreciseStrike_Hit_01 100 100 .8
	End
End
	Condition
		On	Time
		Time	0

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Dust_Medium.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Shockwave.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Flash_Medium.part
			LifeSpan 5
		End

	End

End
