###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################


	Condition
		On		TRIGGERBITS
		TRIGGERBITS	FEMALE
		DoMany		1

		Event
			EName	HugeWaterfall
			At 	FootL
			Type 	Local
			Flags	OneAtATime
			UNTIL	AIR
			CHILDFX	:CHILDFX_WaterfallCape_Splash_Double_FEMALE.fx
		End

	End
End
