#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 140


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_Hit_01 100 100 .8
	End
End



	Condition
		On Time
		Time 0
		Repeat 15

		Event
			At chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
			LifeSpan 100
		End

		Event
			At chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
			HardwareOnly
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 0
		Repeat 2
		RepeatJitter 1

		Event
			At chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
			LifeSpan 90
			LifeSpanJitter 15
		End

		Event
			At chest
			Type Local
			ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
			HardwareOnly
			LifeSpan 90
			LifeSpanJitter 15
		End

	End

End
