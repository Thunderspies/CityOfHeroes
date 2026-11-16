#
FxInfo

Lifespan 60

###########################################################

	Condition
		On		Time
		Time		15

		Event
			At C_Chest
			Type Local
			Part :Staff_Beam_Body.part
			LifeSpan 15
		End

	End

	Condition
		On		Time
		Time		25

		Event
			At C_Head
			Type Local
			Part :Staff_Tip_Glow_Body.part
			Part :Staff_Tip_Star_Body.part
			Part :Staff_Tip_Ring_Body.part
			LifeSpan 5
		End
	End

	Condition
		On		Time
		Time		58

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 1 7
				PyrRotate 20 15 90
				Spin 0 0.5 0
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Large.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Burst_Large.part
			LifeSpan 2
		End
	End
###########################################################

End
