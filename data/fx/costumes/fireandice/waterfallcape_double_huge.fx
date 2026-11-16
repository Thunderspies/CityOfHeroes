###########################################################

FxInfo

###########################################################

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
			Flags	OneAtATime
			CHILDFX	:CHILDFX_WaterfallCape_NOFLY_Double.fx
		End

		Event
			EName	WaterfallIdleL
			At SpadL
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset -0.2 0.15 -0.6
			End
			Part :Waterfall_Idle_Double_HUGE.part
			Part :Waterfall_Idle_Double_Additive_HUGE.part
		End

		Event
			EName	WaterfallIdleR
			At SpadR
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0.2 0.15 -0.6
			End
			Part :Waterfall_Idle_Double_HUGE.part
			Part :Waterfall_Idle_Double_Additive_HUGE.part
		End

		Event
			EName	WaterfallMovementL
			At SpadL
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0 0.5 -0.5
			End
			Part :Waterfall_Base_Movement_Double_HUGE.part
			Part :Waterfall_Movement_Double_Additive_HUGE.part

		End

		Event
			EName	WaterfallMovementR
			At SpadR
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0 0.5 -0.5
			End
			Part :Waterfall_Base_Movement_Double_HUGE.part
			Part :Waterfall_Movement_Double_Additive_HUGE.part
		End



	End

	Condition
		On TriggerBits
		TriggerBits BACKWARD
		DoMany 1

		Event
			EName WaterfallBackwardL
			At Chest
			Type Local
			Flags OneAtATime
			While BACKWARD
			BhvrOverride
				PositionOffset -0.5 0.9 -0.3
			End
			Part :Waterfall_Backward_Double_Huge.part
			Part :Waterfall_Backward_Double_Additive_HUGE.part
		End

		Event
			EName WaterfallBackwardR
			At Chest
			Type Local
			Flags OneAtATime
			While BACKWARD
			BhvrOverride
				PositionOffset 0.5 0.9 -0.3
			End
			Part :Waterfall_Backward_Double_Huge.part
			Part :Waterfall_Backward_Double_Additive_HUGE.part
		End
	End

###########################################################

End
