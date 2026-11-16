#
FxInfo

	LifeSpan 80


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SM_EyeOfTheStorm_Hit_01 100 100 .8
	End
End

	Condition
		On	Time
		Time	0

		Event
			EName	ChestAnchor
			At Chest
			Type Start
			LifeSpan 40
		End
	End

	Condition
		On	Cycle
		Time	7

		Event
			At ChestAnchor
			Type Start
			Part CustomizeablePowers\StaffFighting\Hit_Dust.part
			Part CustomizeablePowers\StaffFighting\Hit_Flash.part
			LifeSpan 5
		End

	End

	Condition
		On	Time
		Time	42

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset	0.0 1.0 0.0
			End
			Part CustomizeablePowers\StaffFighting\Hit_Dust_Large.part
			Part CustomizeablePowers\StaffFighting\Hit_Flash_Large.part
			Part CustomizeablePowers\StaffFighting\Hit_Shockwave_medium.part

			LifeSpan 5
		End

#		Event
#			EName	StaffMind
#			At 	Origin
#			Type	Local
#			CHILDFX	:Hit_Body.fx
#		End
#
#		Event
#			EName	StaffMind
#			At 	Origin
#			Type	Local
#			CHILDFX	:Hit_Mind.fx
#		End
#
#		Event
#			EName	StaffMind
#			At 	Origin
#			Type	Local
#			CHILDFX	:Hit_Soul.fx
#		End

	End

End
