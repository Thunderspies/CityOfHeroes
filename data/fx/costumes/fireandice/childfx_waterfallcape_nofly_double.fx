###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################


	Condition
		On		TRIGGERBITS
		TRIGGERBITS	MALE
		DoMany		1

		Event
			EName	MaleWaterfall
			At 	FootL
			Type 	Local
			Flags	OneAtATime
			UNTIL	FLY
			BhvrOverride
				PositionOffset 0 0 -2
			End
			CHILDFX	:CHILDFX_WaterfallCape_NOAIR_Double.fx
		End

	End


	Condition
		On		TRIGGERBITS
		TRIGGERBITS	FEMALE
		DoMany		1

		Event
			EName	FemaleWaterfall
			At 	FootL
			Type 	Local
			Flags	OneAtATime
			UNTIL	FLY
			BhvrOverride
				PositionOffset 0 0 -2
			End
			CHILDFX	:CHILDFX_WaterfallCape_NOAIR_Double.fx
		End

	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	HUGE
		DoMany		1

		Event
			EName	HugeWaterfall
			At 	FootL
			Type 	Local
			Flags	OneAtATime
			UNTIL	FLY
			BhvrOverride
				PositionOffset 0 0 -2
			End
			CHILDFX	:CHILDFX_WaterfallCape_NOAIR_Double.fx
		End

	End

End
