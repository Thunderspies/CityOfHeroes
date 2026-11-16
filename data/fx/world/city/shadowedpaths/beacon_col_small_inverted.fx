#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			SoundNoRepeat 2
			Bhvr behaviors/Sound/SoundIn3Out5.bhvr
			Sound FirstWard_ShadowyOrb2_Loop 35 35 0.15
			Sound FirstWard_ShadowyOrb3_Loop 35 35 0.15
			Sound FirstWard_ShadowyOrb4_Loop 35 35 0.15
			Sound FirstWard_ShadowyOrb5_Loop 35 35 0.15
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName MaskAncor
			BhvrOverride
				Alpha 255
				Scale 5.0 5.0 5.0
				PositionOffset 0.0 -4.0 -1.0
			End
		End

		Event
			At MaskAncor
			Type Local
			BhvrOverride
				PyrRotate 90 0 0
				Spin 0.0 0.02 0.0
			End
			Part WORLD\City\ShadowedPaths\Beacon_Border_Small_Inverted.part
			Part WORLD\City\ShadowedPaths\Beacon_Swirls_Additive_Small_Inverted.part
			Part WORLD\City\ShadowedPaths\Beacon_Swirls_Small_Inverted.part
		End

		Event
			At MaskAncor
			Type Local
			EName SpinAnchor
			BhvrOverride
				PyrRotate 90 0 0
				Spin 0.0 5.01 0.0
			End
			Part WORLD\City\ShadowedPaths\Beacon_Core_Glow_Inverted.part
			Part WORLD\City\ShadowedPaths\Beacon_Core_Small_Inverted.part
		End

	End

End
