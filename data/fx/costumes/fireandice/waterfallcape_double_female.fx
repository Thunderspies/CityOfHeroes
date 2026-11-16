###########################################################

FxInfo

###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	FEMALE
		DoMany		1

		Event
			EName	FemaleWaterfall
			At Root
			Type Local
			Flags	OneAtATime
			WHILE	FEMALE
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
				PositionOffset -0.2 -0.15 -0.2
			End
			Part :Waterfall_Idle_Double_FEMALE.part
			Part :Waterfall_Idle_Double_Additive_FEMALE.part
		End

		Event
			EName	WaterfallIdleR
			At SpadR
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0.2 -0.15 -0.2
			End
			Part :Waterfall_Idle_Double_FEMALE.part
			Part :Waterfall_Idle_Double_Additive_FEMALE.part
		End

		Event
			EName	WaterfallMovementL
			At SpadL
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0 0.25 -0.3
			End
			Part :Waterfall_Base_Movement_Double_FEMALE.part
			Part :Waterfall_Movement_Double_Additive_FEMALE.part

		End

		Event
			EName	WaterfallMovementR
			At SpadR
			Type Local
			UNTIL	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset 0 0.25 -0.3
			End
			Part :Waterfall_Base_Movement_Double_FEMALE.part
			Part :Waterfall_Movement_Double_Additive_FEMALE.part
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
				PositionOffset -0.2 0.6 -0.3
			End
			Part :Waterfall_Backward_Double_FEMALE.part
			Part :Waterfall_Backward_Double_Additive_FEMALE.part
		End

		Event
			EName WaterfallBackwardR
			At Chest
			Type Local
			Flags OneAtATime
			While BACKWARD
			BhvrOverride
				PositionOffset 0.2 0.6 -0.3
			End
			Part :Waterfall_Backward_Double_FEMALE.part
			Part :Waterfall_Backward_Double_Additive_FEMALE.part

		End
	End

###########################################################

End
