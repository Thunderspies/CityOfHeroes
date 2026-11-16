#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim FX_PaperLantern
			At Root
			Type Local
			EName Prop
			Bhvr Behaviors/FadeOut.bhvr
			BhvrOverride
				#PositionOffset 0 3 3
				PyrRotateJitter 0 120 0
				Scale 0.7 0.7 0.7
				AnimScale .1
			End
		End

	End

End
