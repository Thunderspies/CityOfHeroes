#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type PositOnly
			POther LArmL
			Part CustomizeablePowers\MartialManipulation\Subtractive\Beam_Rings.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 8

		Event
			At HandL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\MartialManipulation\Subtractive\Hand_Trails.part
			LifeSpan 10
		End

		Event
			At HandL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 -0.5 0.0
				PyrRotate 60 30 30
				PyrRotateJitter 10 0 10
			End
		#	Part CustomizeablePowers\MartialManipulation\Subtractive\Hand_Rings_Burst_LocalFacing.part
			Part CustomizeablePowers\MartialManipulation\Subtractive\Beam_Rings.part
			LifeSpan 15
		End

#		Event
#			At HandL
#			Type Local
#			BhvrOverride
#				PositionOffset 0.0 -0.5 0.0
#				PyrRotate 100 0 -30
#				PyrRotateJitter 10 0 10
#			End
#			Part CustomizeablePowers\MartialManipulation\Subtractive\Hand_Rings_Burst_LocalFacing.part
#			Part CustomizeablePowers\MartialManipulation\Subtractive\Beam_Rings.part
#			LifeSpan 14
#		End
#
#		Event
#			At HandL
#			Type Local
#			BhvrOverride
#				PositionOffset 0.0 -0.5 0.0
#				PyrRotate 0 120 0
#				PyrRotateJitter 20 0 20
#			End
#			Part CustomizeablePowers\MartialManipulation\Subtractive\Hand_Rings_Burst_LocalFacing.part
#			Part CustomizeablePowers\MartialManipulation\Subtractive\Beam_Rings.part
#			LifeSpan 13
#		End

	End

	Condition
		On Time
		Time 10

		Event
			At WepL
			Type Local
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Burst_Rapid_Small.part
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 13

		Event
			At WepL
			Type Local
			Part CustomizeablePowers\MartialManipulation\Subtractive\Rings_Shockwave_Small.part
			LifeSpan 3
		End

	End

End
