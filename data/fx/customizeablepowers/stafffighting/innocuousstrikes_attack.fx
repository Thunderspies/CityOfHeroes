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
		Sound 	SM_InnocuousStrike_Attack_01 100 100 .8
	End
End

	Condition
		Time 1

		Event
			At C_Head
			Type Local
			EName StaffAnchor
			BhvrOverride
				PositionOffset 0 1 0
			End
		End

	End

	Condition
		Time 16

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				#PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 80
		End

	End

	Condition
		Time 24

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 5
		End

	End

	Condition
		Time 31

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 5
		End

	End

	Condition
		Time 40

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 5
		End

	End

	Condition
		Time 48

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 5
		End

	End

	Condition
		Time 55

		Event
			At StaffAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Up.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Up_Lingering.part
			LifeSpan 5
		End

	End

End
