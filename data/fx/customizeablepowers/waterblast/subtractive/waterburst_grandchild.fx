#
FxInfo


###########################################################

	Condition
		On Cycle
		Time 3
		Chance 0.5

		Event
			At Root
			Type Local
			EName InitSplash
			BhvrOverride
				PositionOffset 0 0.5 0
				StartJitter 10 0 10
			End
			Part :RainDrop_Splash.part
			LifeSpan 3
		End

	End

End
