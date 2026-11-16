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
		Sound 	SM_GuardedSpin_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 5

		Event
			At WepR
			Type Local
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PyrRotateJitter 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Highlights.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_CC.part
			LifeSpan 20
		End

	End

	Condition
		Time 41

		Event
			At WepR
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
				Spin 0 0.4 0
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Burst_Additive.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_Burst.part
			LifeSpan 10
		End

	End

	Condition
		Time 40

		Event
			At WepR
			Type Local
			Bhvr behaviors\GenericParticleFadeQuick.bhvr
			BhvrOverride
				PyrRotate 0 0 90
				Spin 0 0.8 0
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_Burst.part
			LifeSpan 3
		End

	End

End
