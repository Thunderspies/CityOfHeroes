#                                                          
FxInfo

	LifeSpan 90


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Pow
			Part :Clawflash.part
			Part :ClawEmbers.part
			Part :ClawEmbersBroad.part
			Part :ClawsStar.part
			Sound EviscerHit2 100.0 100.0 0.8
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 0
		Repeat 5
		RepeatJitter 3

		Event
			At Chest
			Type Start
			ChildFx :MetalSparkUP.fx
			EName Sparkup
		End

		Event
			At Chest
			Type Start
			ChildFx :MetalSparkOUT.fx
			EName Sparkfar
			HardwareOnly 
		End

	End

	Condition
		On Time
		Time 0
		Repeat 4
		RepeatJitter 2

		Event
			At Chest
			Type Start
			ChildFx :MetalSparkOUTbig.fx
			EName Sparkfar
		End

	End

	Condition
		On Time
		Time 2
		Repeat 4
		RepeatJitter 3

		Event
			At Chest
			Type Start
			ChildFx :MetalSparkOUT.fx
			EName Sparkout
		End

	End

End
