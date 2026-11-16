#                                                          
FxInfo

	LifeSpan 45


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SM_GuardedSpin_Hit_01 100 100 .8
	End
End
	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Start
			EName ChestAnchor
			LifeSpan 30
		End

	End

	Condition
		On Cycle
		Time 5

		Event
			At ChestAnchor
			Type Start
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Dust.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Flash.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 37

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Dust_Medium.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Flash_Medium.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Shockwave.part
			LifeSpan 5
		End

	End

End
