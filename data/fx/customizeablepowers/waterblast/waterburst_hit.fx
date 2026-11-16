#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_WaterBurst_Hit 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Part :WaterBurst_InitialSplash_01.part
			Part :WaterBurst_InitialSplash_02.part
			LifeSpan 18
		End

	End

	Condition
		On Time
		Time 8

		Event
			At Origin
			Type Local
			#ChildFx :WaterBurst_Bubble_Child.fx
			BhvrOverride
				PositionOffset 0 1 0
			End
			LifeSpan 18
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			LifeSpan 50
			ChildFX :WaterBurst_Burst_Child.fx
		End

	End

End
