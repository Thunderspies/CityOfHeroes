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
			UNTIL	AIR
			CHILDFX	:CHILDFX_WaterfallCape_Splash.fx
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
			UNTIL	AIR
			CHILDFX	:CHILDFX_WaterfallCape_Splash_FEMALE.fx
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
			UNTIL	AIR
			CHILDFX	:CHILDFX_WaterfallCape_Splash_HUGE.fx
		End

	End
End
