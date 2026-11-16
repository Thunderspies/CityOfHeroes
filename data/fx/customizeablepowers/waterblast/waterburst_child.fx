#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Waterburst_Initial_Cold_02.part
			#Part :Waterburst_Initial_Cold_03.part
		End

	End

	Condition
		On Time
		Time 8

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			ChildFX	:WaterBurst_GrandChild.fx
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			ChildFX	:WaterBurst_GrandChild.fx
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			ChildFX	:WaterBurst_GrandChild.fx
		End

	End

End
