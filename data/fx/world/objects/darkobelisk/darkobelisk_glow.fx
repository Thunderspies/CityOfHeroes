#
FxInfo


###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Lava_Obelisk_Loop 200 200 .3
	End
End

	Condition
		On Time
		Time 0
		Chance 0.0000

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 5 0
			End
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
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightningA.part
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightningASmall.part
			LifeSpan 0
			LifeSpanJitter 0
		End

	End

	Condition
		On Time
		Time 0

		Event
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_InnerGlowA.part
			Part WORLD\Objects\DarkObelisk\DarkObelisk_GlowFront.part
			LifeSpan 0
			LifeSpanJitter 0.0000
		End

	End

End
