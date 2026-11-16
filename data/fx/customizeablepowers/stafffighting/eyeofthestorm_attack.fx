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
		Sound 	SM_EyeOfTheStorm_Attack_01 100 100 .8
	End
End

	Condition
		Time 10

		Event
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
				Spin 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Highlights.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_CC.part
			LifeSpan 5
		End

	End

	Condition
		Time 20

		Event
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Expanding.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Highlights.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_CC.part
			LifeSpan 5
		End

#		Event
#			EName	StaffSoul
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Soul.fx
#		End
#
#		Event
#			EName	StaffMind
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Mind.fx
#		End
#
#		Event
#			EName	StaffBody
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Body.fx
#		End
	End

	Condition
		Time 30

		Event
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Expanding.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Highlights.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_CC.part
			LifeSpan 5
		End
#
#		Event
#			EName	StaffSoul
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Soul.fx
#		End
#
#		Event
#			EName	StaffMind
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Mind.fx
#		End
#
#		Event
#			EName	StaffBody
#			At 	Origin
#			Type	Local
#			CHILDFX	:CHILD_EyeOfTheStorm_Body.fx
#		End
	End

	Condition
		Time 40

		Event
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
				Spin 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Highlights.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_CC.part
			LifeSpan 5
		End

	End

	Condition
		Time 45

		Event
			At C_Head
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
				Spin 0 0 0
			End
			Part CustomizeablePowers\StaffFighting\Ground_Dust_Point.part
			LifeSpan 21
		End

		Event
			At Root
			Type Start
			Part CustomizeablePowers\StaffFighting\Shockwave_OuterRing_Spin.part
			Part CustomizeablePowers\StaffFighting\Shockwave_Fill_Spin.part
			Part CustomizeablePowers\StaffFighting\Shockwave_Spin.part
			LifeSpan 14
		End

#		Event
#			EName	BodyShockwave
#			At Root
#			Type Start
#			CHILDFX	:CHILD_EyeOfTheStorm_Body2.fx
#			LifeSpan 14
#		End
#
#		Event
#			EName	MindShockwave
#			At Root
#			Type Start
#			CHILDFX	:CHILD_EyeOfTheStorm_Mind2.fx
#			LifeSpan 14
#		End

#		Event
#			EName	SoulShockwave
#			At Root
#			Type Start
#			CHILDFX	:CHILD_EyeOfTheStorm_Soul2.fx
#			LifeSpan 14
#		End
	End

End
