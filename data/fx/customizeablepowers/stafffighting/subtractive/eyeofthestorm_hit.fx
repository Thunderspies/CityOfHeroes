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
		On Time
		Time 0

		Event
			At Chest
			Type Start
			EName ChestAnchor
			LifeSpan 40
		End

	End

	Condition
		On Cycle
		Time 7

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
		Time 42

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Dust_Large.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Flash_Large.part
			Part CustomizeablePowers\StaffFighting\Subtractive\Hit_Shockwave_medium.part
			LifeSpan 5
		End

		#Event
			#At Origin
			#Type Local
			#ChildFx :Hit_Body.fx
			#EName StaffMind
		#End

		#Event
			#At Origin
			#Type Local
			#ChildFx :Hit_Mind.fx
			#EName StaffMind
		#End

		#Event
			#At Origin
			#Type Local
			#ChildFx :Hit_Soul.fx
			#EName StaffMind
		#End

	End

End
