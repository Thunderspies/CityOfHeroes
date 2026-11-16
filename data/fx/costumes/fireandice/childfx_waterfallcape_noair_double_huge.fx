###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################


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
			CHILDFX	:CHILDFX_WaterfallCape_Splash_Double_HUGE.fx
		End

	End
End
