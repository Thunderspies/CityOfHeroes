#############################################################
## Fish_School_01.fx
#############################################################

FxInfo

###########################################################

Condition
		On Time
		Time 0

	Event
			At Root
			Type Local
			EName FishAnchor
			BhvrOverride
				PositionOffset 0 0 5
			End
		End

	Event
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish01
			BhvrOverride
				PositionOffset 1 -1 0.5
				Scale	0.1 0.1 0.1
			End
		End

	Event
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish02
			BhvrOverride
				PositionOffset 0.5 -0.2 1
				Scale	0.1 0.1 0.1
			End
		End

	Event
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish03
			BhvrOverride
				PositionOffset 1.5 0.2 0
				Scale	0.1 0.1 0.1
			End
		End

	Event
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish04
			BhvrOverride
				PositionOffset -0.2 0.5 0.5
				Scale	0.07 0.07 0.07
			End
		End

	Event
			Anim FX_Fish_01
			At FishAnchor
			Type Local
			EName Fish05
			BhvrOverride
				PositionOffset 0 -0.5 -1
				Scale	0.08 0.08 0.08
			End
		End
	End
End
