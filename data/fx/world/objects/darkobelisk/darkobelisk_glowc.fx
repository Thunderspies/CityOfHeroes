#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0
		Chance 0.0000

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 -100 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_DarkCloudC.part
			LifeSpan 0
			LifeSpanJitter 0
		End

	End

	Condition
		On Time
		Time 0

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 -5 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightningC.part
			LifeSpan 0
			LifeSpanJitter 0
		End

	End

	Condition
		On Time
		Time 0

		Event
			Type Local
			At Hips
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_InnerGlowC.part
			Part WORLD\Objects\DarkObelisk\DarkObelisk_GlowFront.part
			LifeSpan 0
			LifeSpanJitter 0.0000
		End

	End

End
