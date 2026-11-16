#
FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Fire.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Fire_Add.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Hellfire_Highlight.part
			LifeSpan 65
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Fire.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Fire_Add.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Hands_Hellfire_Highlight.part
			LifeSpan 65
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Chest
			Type PositOnly
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars2.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Smoke_Out.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Smoke_Fall.part
			LifeSpan 20
		End

		Event
			At Chest
			Type PositOnly
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Glow_Core.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Smoke_Hit.part
			LifeSpan 70
		End

		Event
			At Root
			Type Start
			EName PRIME
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 8.15 0.0
				InitialVelocity 0.0 -0.1 0.0
			End
			LifeSpan 60
		End

		Event
			At PRIME
			Type Local
			EName SpinnerAnchor
			BhvrOverride
				Spin 0 -0.08 0
				SpinJitter 0 0 0
			End
		End

		Event
			At SpinnerAnchor
			Type Local
			EName WispAnchor1
			BhvrOverride
				PositionOffset 2 -0.5 0.0
			End
			LifeSpan 80
		End

		Event
			At SpinnerAnchor
			Type Local
			EName WispAnchor2
			BhvrOverride
				PositionOffset -2 -0.5 0.0
			End
			LifeSpan 80
		End

		Event
			Ename RUNEFLOAT1
			At WispAnchor1
			Type Posit
			Bhvr behaviors\GenericParticleFade.bhvr
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Rune.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Rune_Blur.part
			LifeSpan 80
		End

		Event
			Ename RUNEFLOAT2
			At WispAnchor2
			Type Posit
			Bhvr behaviors\GenericParticleFade.bhvr
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Rune.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Rune_Blur.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Chest
			Type PositOnly
			Part CustomizablePowers_Pools\Sorcery\Subtractive\TwinkleStars_In.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\TwinkleStars_In2.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Ward_Glow.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\RadStreaks_In.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Smoke_Fall.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 88

		Event
			Ename GROUNDRUNES
			Type Local
			At  Root
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 0.025 0
				PositionOffset 0 3 0
			End
			Part CustomizablePowers_Pools\Sorcery\Subtractive\RingExpand.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Circle_Runes.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Circle_Runes_Blur.part
		End

		Event
			At Hips
			Type PositOnly
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Protection_Runes01.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Protection_Runes02.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Protection_Runes03.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Static_TwinkleStars.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Static_TwinkleStars2.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Protection_Smoke.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Protection_Smoke_Add.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Glow_out.part
			Part CustomizeablePowers\Brawling\Subtractive\Hit_Flash_Big.part
		End
	End


End
