#
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0 0 -1
				Scale 5 5 5
				Alpha 255
			End
			Sound Apparitions_CrushingFate_Loop 100 100 0.8
		End

		Event
			At Root
			Type Start
			EName CrushingHateSwirls
			BhvrOverride
				PyrRotate 90 180 0
				Spin 0.0 0.02 0.0
			End
			Part :Beacon_Border_NW.part
			Part :Beacon_DarkSpot_NW.part
			Part :Beacon_Swirls_Additive_NW.part
		End

		Event
			At CrushingHateSwirls
			Type Local
			EName RightArm
			BhvrOverride
				PositionOffset 1.5 0 0
				PyrRotate 0 0 0
			End
			Part :Beacon_Wisps_NW.part
			Part :Beacon_Wisps_Glow_NW.part
		End

		Event
			At CrushingHateSwirls
			Type Local
			EName LeftArm
			BhvrOverride
				PositionOffset -1.5 0 0
				PyrRotate 0 180 0
			End
			Part :Beacon_Wisps_NW.part
			Part :Beacon_Wisps_Glow_NW.part
		End

	End

	Condition
		On Time
		Time 50

		Event
			At CrushingHateSwirls
			Type Start
			BhvrOverride
				PyrRotate 0 0 0
				Spin 0.0 0.015 0.0
			End
			Part :Beacon_Particles_Stop_NW.part
		End

	End

End
