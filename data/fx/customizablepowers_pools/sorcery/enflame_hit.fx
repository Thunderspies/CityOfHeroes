#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SorceryPowerPool_Enflame_Hit_01 100 100 1
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type PositOnly
			Part :Enchant_Rune_TwinkleStars.part
			Part :Enchant_Rune_TwinkleStars2.part
			Part :Enflame_Rune_Smoke_Out.part
			LifeSpan 3
		End

		Event
			At Chest
			Type PositOnly
			Part :TwinkleStars_In.part
			Part :TwinkleStars_In2.part
			Part :Enchant_Rune_Smoke_Fall.part
			Lifespan 20
		End

		Event
			At Root
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
			EName PRIME
			Bhvr behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 5 0.0
				InitialVelocity 0.0 -0.05 0.0
			End
			LifeSpan 60
		End

		Event
			At PRIME
			Type Local
			EName WispAnchor1
			BhvrOverride
				PositionOffset 2 -0.5 0.0
			End
			LifeSpan 80
		End

		Event
			At PRIME
			Type Local
			EName WispAnchor2
			BhvrOverride
				PositionOffset -2 -0.5 0.0
			End
			LifeSpan 80
		End

		Event
			At WispAnchor1
			Type Posit
			Bhvr behaviors\GenericParticleFade.bhvr
			Part :Ward_Rune.part
			Part :Ward_Rune_Blur.part
			LifeSpan 80
		End

		Event
			At WispAnchor2
			Type Posit
			Bhvr behaviors\GenericParticleFade.bhvr
			Part :Ward_Rune.part
			Part :Ward_Rune_Blur.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Root
			Type PositOnly
			LifeSpan 70
		End

		Event
			At Root
			Type PositOnly
			Part :Ward_Glow_Core.part
			Part :TwinkleStars_In.part
			Part :TwinkleStars_In2.part
			Part :Ward_Glow.part
			Part :RadStreaks_In.part
			LifeSpan 45
		End

	End

End
