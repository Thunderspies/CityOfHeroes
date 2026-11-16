#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 3

		Event
			At Root
			Type Local
			Sound SM_PreciseStrike_Attack_01 200 100 0.8
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Start
			EName ChestAnchor
			Sound Brawling_SpinningStrike_Attack_01 200 100 0.8
		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepL
			Type Local
			Bhvr behaviors\GenericParticleFadeQuick.bhvr
			BhvrOverride
				#PositionOffset 0 0 0
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Ends_Swish_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Root
			Type Local
			#Bhvr behaviors\GenericParticleFadeQuick.bhvr
			BhvrOverride
				PositionOffset 0 1 6
				PyrRotate 20 15 90
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrongFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_FrontFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_FrontFacing_Large.part
			LifeSpan 1
		End

	End

End
