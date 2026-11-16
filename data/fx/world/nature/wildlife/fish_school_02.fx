#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName Rotator
			BhvrOverride
				PositionOffset 0 0 0
				Spin 0 -0.005 0
			End
		End

		Event
			At Rotator
			Type Local
			EName FishAnchor
			BhvrOverride
				PositionOffset 40 0 0
				Scale	0.2 0.2 0.2
				Alpha	1
			End
			Geom FishSchool_Offsets_01
		End

		Event
			AltPiv 1
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish01
			BhvrOverride
				#PositionOffset 0 0 0
				#Scale 0.01 0.01 0.1
			End
		End

		Event
			AltPiv 2
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish02
			BhvrOverride
				PositionOffset 0.5 -0.2 2
				#Scale 0.01 0.01 0.01
			End
		End

		Event
			AltPiv 3
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish03
			BhvrOverride
				PositionOffset 1.5 0.2 0
				#Scale 0.01 0.01 0.01
			End
		End

		Event
			AltPiv 4
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish04
			BhvrOverride
				PositionOffset -0.2 0.5 0.5
				#Scale 0.07 0.07 0.07
			End
		End

		Event
			AltPiv 5
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish05
			BhvrOverride
				PositionOffset 0 -0.5 -1
				#Scale 0.08 0.08 0.08
			End
		End

		Event
			AltPiv 6
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish06
			BhvrOverride
				PositionOffset 0 -0.5 -1
				#Scale 0.08 0.08 0.08
			End
		End

		Event
			AltPiv 7
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish07
			BhvrOverride
				PositionOffset 0 -0.5 -1
				#Scale 0.08 0.08 0.08
			End
		End

		Event
			AltPiv 8
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish08
			BhvrOverride
				PositionOffset 0 -0.5 -1
				#Scale 0.08 0.08 0.08
			End
		End

	End

End
