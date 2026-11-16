###########################################################

FxInfo

###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	MALE
		DoMany		1

		Event
			EName	MaleWaterfall
			At Root
			Type Local
			Flags	OneAtATime
			WHILE	MALE
			CHILDFX	:CHILDFX_WaterfallCape_NOFLY.fx
		End

		Event
			EName	WaterfallHead
			At Chest
			Type Local
			Flags	OneAtATime
			UNTIL	BACKWARD

			BhvrOverride
				PositionOffset 0 0.25 -0.35
			End
			Part :Waterfall_Idle.part
			Part :Waterfall_Idle_Additive.part

		End

		Event
			EName	WaterfallTail
			At Chest
			Type Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				FadeOutLength	5
				PositionOffset 0 0.8 -0.4
			End
			Part :Waterfall_Movement.part
			Part :Waterfall_Base.part

		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	FEMALE
		DoMany		1

		Event
			EName	FemaleWaterfall
			At 	Root
			Type 	Local
			Flags	OneAtATime
			WHILE	FEMALE
			CHILDFX	:CHILDFX_WaterfallCape_NOFLY.fx
		End

		Event
			EName	WaterfallHead
			At 	Chest
			Type 	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset 0 0.1 -0.3
			End
			Part :Waterfall_Idle.part
			Part :Waterfall_Idle_Additive.part

		End

		Event
			EName	WaterfallTail
			At 	Chest
			Type 	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset 0 0.5 -0.35
			End
			Part :Waterfall_Movement.part
			Part :Waterfall_Base.part
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	HUGE
		DoMany		1

		Event
			EName	HugeWaterfall
			At Root
			Type Local
			Flags	OneAtATime
			WHILE	HUGE
			CHILDFX	:CHILDFX_WaterfallCape_NOFLY.fx
		End

		Event
			EName	WaterfallHead
			At Chest
			Type Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset 0 0.5 -0.65
			End
			Part :Waterfall_Idle.part
			Part :Waterfall_Idle_Additive.part
		End

		Event
			EName	WaterfallTail
			At 	Chest
			Type 	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset 0 1.0 -0.65
			End
			Part :Waterfall_Movement_Huge.part
			Part :Waterfall_Base.part

		End
	End



	Condition
		On TriggerBits
		TriggerBits BACKWARD
		DoMany 1

		Event
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 0.8 -0.3
			End
			Part :Waterfall_Movement_Back.part

			EName Cloud
			Flags OneAtATime
			While BACKWARD
		End
	End

###########################################################

End
