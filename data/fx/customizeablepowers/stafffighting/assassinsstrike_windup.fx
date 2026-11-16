#
FxInfo

	LifeSpan 100


###########################################################

Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	SM_AssassinsStaff_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 36

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 0 90 0
			End
			Part CustomizeablePowers\StaffFighting\WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 0 -90 0
			End
			Part CustomizeablePowers\StaffFighting\WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 90 90 0
			End
			Part CustomizeablePowers\StaffFighting\WaveyLines_Shrink.part
			LifeSpan 35
		End

		Event
			At C_Chest
			Type Local
			Bhvr	Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate -90 -90 0
			End
			Part CustomizeablePowers\StaffFighting\WaveyLines_Shrink.part
			LifeSpan 35
		End

	End

End
