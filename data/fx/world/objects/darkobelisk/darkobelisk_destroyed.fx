#
FxInfo

	LifeSpan 0


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Lava_ObeliskSmoking_Loop 100 100 .1
	End
End

	Condition
		On Time
		Time 0

		Event
			Type Start
			At Hips
			BhvrOverride
				PositionOffset 0 -55 0
			End
			Part WORLD\Objects\DarkObelisk\DarkObelisk_DestroyedCloud.part
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
			Part WORLD\Objects\DarkObelisk\DarkObeliskLightning_Damaged.part
			LifeSpan 0
			LifeSpanJitter 0
		End

	End

End
