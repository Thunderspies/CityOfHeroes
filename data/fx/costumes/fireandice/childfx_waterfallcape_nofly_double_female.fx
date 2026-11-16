###########################################################

FxInfo

	Input
		InpName Root
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
			UNTIL	FLY
			BhvrOverride
				PositionOffset 0 0 -2
			End
			CHILDFX	:CHILDFX_WaterfallCape_NOAIR_Double_FEMALE.fx
		End

	End

###########################################################

End
