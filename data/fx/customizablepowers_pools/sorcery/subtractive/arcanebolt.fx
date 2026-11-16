#
FxInfo

	LifeSpan 100


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
		Time 20

		Event
			At Root
			Type Start
			EName ConeOffset
			Geom FX_ConeOffsets
		End

		Event
			AltPiv 1
			At ConeOffset
			Type Local
			EName RuneAnchor
			BhvrOverride
				PositionOffset 0 5 3
			End
		End

		Event
			At RuneAnchor
			Type Local
			BhvrOverride
				PyrRotate -90 180 0
			End
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Arcane_Rune_Smoke.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Arcane_Rune_Smoke_Add.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At RuneAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate -90 180 0
			End
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Arcane_Rune.part
		End

	End

	Condition
		On Time
		Time 51

		Event
			At RuneAnchor
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			POther T_Chest
			Bhvr behaviors\BeanBagProjectile.bhvr
			BhvrOverride
				TrackRate 3
			End
			LifeSpan 80
		End

		Event
			At Prime
			Type Posit
			EName PrimeHit2
			BhvrOverride
				PyrRotate -90 180 0
			End
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Projectile.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Projectile_Add.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_Blur_Projectile_Add.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_SmokeTrail.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_SmokeTrail2.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars2.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName PrimeHit2
		End

	End

End
