#
FxInfo

	LifeSpan 720


###########################################################

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type StartPositOnly
			Bhvr :Soulshatter_Ring_Pricks.bhvr
			Geom FX_Impale_RainColumn2
			LifeSpan 660
		End

		Event
			At T_Root
			Type StartPositOnly
			EName IcicleCentral
			Bhvr :Soulshatter_Ring.bhvr
			Geom FX_SoulShatter
			LifeSpan 660
		End

		Event
			At T_Root
			Type StartPositOnly
			Bhvr :Soulshatter_Ring.bhvr
			BhvrOverride
				PositionOffset 0 1 0
				Alpha 255
				StartColor 20 15 30
			End
			Geom FX_SoulShatter_Alpha
			LifeSpan 675
		End

		Event
			At T_Root
			Type StartPositOnly
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Soulshatter_Hurricane_Highlight.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Soulshatter_Hole.part
			sound DD_SoulshatterPet_Loop 100 100 .65
			LifeSpan 675
		End

	End

End
