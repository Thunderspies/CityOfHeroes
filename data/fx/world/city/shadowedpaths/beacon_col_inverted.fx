#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Hips
			Type Local
			EName BeaconAnchor
			Bhvr behaviors/Sound/SoundIn3Out5.bhvr
			BhvrOverride
				PositionOffset 0 -4 0
			End
			Sound FirstWard_ShadowyOrb1_Loop 50 50 0.15
		End

		#Event
			#At BeaconAnchor
			#Type Local
			#BhvrOverride
				#StartColor 1 1 1
				#Alpha 255
				#Scale 5.0 5.0 5.0
				#PositionOffset 0.0 0.0 0.1
			#End
			#Geom GEO_Eye_CoL_Mask_01
		#End

		Event
			At BeaconAnchor
			Type Local
			EName SpinAnchor
			BhvrOverride
				PyrRotate 90 0 0
				Spin 0.0 0.02 0.0
			End
			Part :Beacon_Border_Inverted.part
			Part :Beacon_Swirls_Inverted.part
			Part WORLD\City\ShadowedPaths\Beacon_Rays.part
			Part :Beacon_Swirls_Additive_Inverted.part
		End

		Event
			At SpinAnchor
			Type Local
			EName RightArm
			BhvrOverride
				PositionOffset 0 0 -1.5
				PyrRotate 0 90 0
			End
			Part :Beacon_Wisps_Inverted.part
			Part :Beacon_Wisps_Glow_Inverted.part
		End

		Event
			At SpinAnchor
			Type Local
			EName LeftArm
			BhvrOverride
				PositionOffset 0 0 1.5
				PyrRotate -0 -90 0
			End
			Part :Beacon_Wisps_Inverted.part
			Part :Beacon_Wisps_Glow_Inverted.part
		End

		Event
			At BeaconAnchor
			Type Local
			EName SpinAnchor
			BhvrOverride
				PyrRotate 90 0 0
				Spin 0.0 0.01 2.0
			End
			Part :Beacon_Core_Inverted.part
			Part :Beacon_Core_Glow_Inverted.part
		End

	End

	Condition
		On Time
		Time 50

		Event
			At BeaconAnchor
			Type Local
			BhvrOverride
				PyrRotate 90 0 0
				Spin 0.0 0.015 0.0
			End
			Part :Beacon_Particles_Stop_Inverted.part
		End

	End

End
