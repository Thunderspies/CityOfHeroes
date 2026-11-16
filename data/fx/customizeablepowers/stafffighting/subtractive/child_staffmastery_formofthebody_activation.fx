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
			Part CustomizeablePowers\StaffFighting\Ends_Swish_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrontFacing.part
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing.part
			LifeSpan 9
		End

	End

	#Condition
		#Time 8

		#Event
			#At WepR
			#Type Local
			#BhvrOverride
				#PyrRotate 0 0 50
				#PyrRotateJitter 10 10 10
			#End
			#Part CustomizeablePowers\StaffFighting\Center_Swish_Body.part
			#Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Shockwave.part
			#Part CustomizeablePowers\StaffFighting\Center_Swish_Mind.part
			#Part CustomizeablePowers\StaffFighting\Center_Swish_Soul.part
			#LifeSpan 5
		#End

	#End

	Condition
		Time 19

		Event
			At C_Chest
			Type Local
			Part :Staff_Beam_Body.part
			#Geom GEO_FX_TestSphere
			LifeSpan 50
		End

	End

	Condition
		Time 31

		Event
			At C_Head
			Type Local
			Part :Staff_Tip_Star_Body.part
			Part :Staff_Tip_Glow_Body.part
			Part :Staff_Tip_Ring_Body.part
			LifeSpan 5
		End

	End

End
