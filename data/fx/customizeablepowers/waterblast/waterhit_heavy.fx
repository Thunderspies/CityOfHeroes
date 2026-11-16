#
FxInfo

	LifeSpan 120


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_WaterHit_Heavy 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :Hit_Ring_Heavy.part
			Part :Hit_Flash_Big.part
			Part :Hit_Flash_Heavy.part
			Part :Hit_Glow_Heavy.part
			Part :Hit_Streaks_Big_Continuing.part
			Part :Splash_Splatter_Big_Continuing.part
			Part :Splash_Splatter_Add_Big_Continuing.part
			Part :Splash_Splatter2_Big_Continuing.part
			Part :Splash_Splatter2_Add_Big_Continuing.part
			Part :Splash_Splatter3_Big_Continuing.part
			Part :Splash_Spray_Big_Continuing.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 1
		Random 1
		Chance 0.7

		Event
			At Head
			Type PositOnly
			EName Drips1
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
		End

		Event
			At UArmL
			Type PositOnly
			EName Drips1
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
		End

		Event
			At UArmR
			Type PositOnly
			EName Drips2
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
		End

		Event
			At LArmL
			Type PositOnly
			EName Drips3
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At LArmR
			Type PositOnly
			EName Drips4
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At ULegR
			Type PositOnly
			EName Drips5
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At ULegL
			Type PositOnly
			EName Drips6
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At Chest
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Chest.part
			LifeSpan 5
			LifeSpanJitter 2
		End

	End

	Condition
		On Cycle
		Time 2
		Random 1

		Event
			At UArmL
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At UArmR
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At LLegR
			Type Local
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At LLegL
			Type Local
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At HandL
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At HandR
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Arms.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At LLegR
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Foot.part
			LifeSpan 5
			LifeSpanJitter 2
		End

		Event
			At LLegL
			Type PositOnly
			EName AOERing
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Drips_Foot.part
			LifeSpan 5
			LifeSpanJitter 2
		End

	End

	Condition
		On Cycle
		Time 5
		Repeat 1
		RepeatJitter 1

		Event
			At Root
			Type Start
			EName Splashes
			BhvrOverride
				StartJitter 2 0 1
			End
			Part :RainDrop_Splash_Small.part
			LifeSpan 1
		End

	End

End
