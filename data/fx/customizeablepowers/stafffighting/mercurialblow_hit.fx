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
		Sound 	SM_MercurialBlow_Hit_01 100 100 .8
	End
End

	Condition
		On	Time
		Time	0

		Event
			At Chest
			Type Start
			Part CustomizeablePowers\StaffFighting\Hit_Dust.part
			Part CustomizeablePowers\StaffFighting\Hit_Flash.part
			LifeSpan 5
		End

	End

End
