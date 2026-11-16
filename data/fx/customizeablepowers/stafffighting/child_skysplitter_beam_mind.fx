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
			Part :Staff_Beam_Mind.part
			LifeSpan 15
		End

	End


	Condition
		On		Time
		Time		15

		Event
			At C_Head
			Type Local
			Part :Staff_Tip_Glow_Mind.part
			Part :Staff_Tip_Star_Mind.part
			Part :Staff_Tip_Ring_Mind.part
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
			Part CustomizeablePowers\StaffFighting\Center_Swish_Mind_Large.part
			LifeSpan 2
		End
	End
###########################################################

End
