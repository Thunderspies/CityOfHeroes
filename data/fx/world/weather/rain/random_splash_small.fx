#                                                          
FxInfo


###########################################################

	Condition
		On Cycle
		Time 120
		Chance 0.300000

		Event
			At Root
			Type Local
			BhvrOverride
				#PositionOffset 0 4.5 5
			End
			Part WORLD\Weather\Rain\RainDrop_Splash_Fast.part
			LifeSpan 5
		End

	End

End
