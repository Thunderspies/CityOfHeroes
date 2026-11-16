#
FxInfo

	LifeSpan 50


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SM_FormOf_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	22
	Event
		Type	Local
		At	Root
		Sound 	SM_FormOf_Hit_01 100 100 .5
	End
End

	Condition
		On Time
		Time 4

		Event
			At WepL
			Type Local
			Bhvr behaviors\GenericParticleFadeQuick.bhvr
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part :Ends_Swish_FrontFacing.part
			Part :Ends_Swish_Bright_FrontFacing.part
			Part :Center_StaffImage_FrontFacing.part
			LifeSpan 9
		End

	End

	Condition
		Time 15

		Event
			At C_Chest
			Type Local
			Part :Staff_Beam_Mind.part
			#Geom GEO_FX_TestSphere
			LifeSpan 50
		End

	End

	Condition
		Time 28

		Event
			At C_Head
			Type Local
			Part :Staff_Tip_Glow_Mind.part
			Part :Staff_Tip_Star_Mind.part
			Part :Staff_Tip_Ring_Mind.part
			LifeSpan 5
		End

	End

End
