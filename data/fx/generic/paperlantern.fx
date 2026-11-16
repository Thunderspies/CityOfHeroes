#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName Prop
			Bhvr Behaviors/FadeOut.bhvr
			BhvrOverride
				Scale 1.5 1.5 1.5
			End
			Geom NW_floating_lantern
		End

		Event
			At Hips
			Type Local
			Bhvr Behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 -0.3 0
			End
			Part GENERIC\Lantern_Glow.part
		End

	End

End
