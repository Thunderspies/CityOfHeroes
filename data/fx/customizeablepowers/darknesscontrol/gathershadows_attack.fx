#                                                          
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 5
		Random 0
		DoMany 0

		Event
			At Chest
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Part :GatherShadows_Tendrils.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 10
		Random 0
		DoMany 0

		Event
			At Chest
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Part :GatherShadows_Puddle.part
			Part :GatherShadows_Smoke.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 15
		Random 0
		DoMany 0

		Event
			At Chest
			Type Start
			ChildFx Powers\BuildUpPowerBoost.fx
			EName Prime
			LookAt Target
			Magnet Target
			LifeSpan 20
		End

	End

End
