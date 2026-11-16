#
FxInfo

	LifeSpan 100


###########################################################
Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	SorceryPowerPool_Enflame_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Enchant_Hands_Fire.part
			Part :Enchant_Hands_Fire_Add.part
			Part :Enchant_Hands_Hellfire_Highlight.part
			LifeSpan 65
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Enchant_Hands_Fire.part
			Part :Enchant_Hands_Fire_Add.part
			Part :Enchant_Hands_Hellfire_Highlight.part
			LifeSpan 65
		End

	End

	Condition
		On Time
		Time 15

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
			Part :Enflame_Rune_Smoke.part
			Part :Enflame_Rune_Smoke_Add.part
		End

	End

	Condition
		On Time
		Time 25

		Event
			At RuneAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate -90 180 0
			End
			Part :Enflame_Rune.part
			Lifespan 15
		End

	End

	Condition
		On Time
		Time 51

		Event
			At RuneAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Enchant_Rune_TwinkleStars.part
			Part :Enchant_Rune_TwinkleStars2.part
			Part :Enflame_Rune_Smoke_Out.part
			Part :Enchant_Rune_Smoke_Fall.part
			Lifespan 3
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
				TrackRate 1
			End
			LifeSpan 80
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End
	End

End
