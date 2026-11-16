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
		Sound 	SM_SkySplitter_Hit_01 100 100 .8
	End
End


	Condition
		On	Time
		Time	0

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\StaffFighting\Hit_Dust.part
			Part CustomizeablePowers\StaffFighting\Hit_Shockwave.part
			Part CustomizeablePowers\StaffFighting\Hit_Flash.part
			LifeSpan 5
		End
End

	Condition
		On	Time
		Time	27
		Event
			At Chest
			Type Start
			Part CustomizeablePowers\StaffFighting\Hit_Dust_Large.part
			Part CustomizeablePowers\StaffFighting\Hit_Shockwave.part
			Part CustomizeablePowers\StaffFighting\Hit_Ring.part
			Part CustomizeablePowers\StaffFighting\Hit_Flash_Large.part
			LifeSpan 5
		End
	End

End
