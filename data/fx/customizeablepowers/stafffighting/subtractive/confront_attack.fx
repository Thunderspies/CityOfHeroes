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
		Sound 	SM_Taunt_Attack_01 100 100 .6
	End
End

	Condition
		On Time
		Time 16

		Event
			At WepR
			Type Local
			#Bhvr behaviors\GenericParticleFadeQuick.bhvr
			BhvrOverride
				PositionOffset 0 0 0
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Confront_Swish_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Confront_Swish_Highlights_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Confront_StaffImage_FrontFacing.part
			LifeSpan 5
		End

	End

End
