#
FxInfo
Lifespan 40


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			Part Emotes\Fireworks\Bloom_Flash_GREEN.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type Start
			Part Emotes\Fireworks\Bloom_Smoke_GREEN.part
			Part Emotes\Fireworks\Bloom_Flash_Glow_GREEN.part
			Part Emotes\Fireworks\Sparkle_Glow_GREEN.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Origin
			Type Start
			EName SparkleAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Drag 0.05
				Gravity 0.0015
				InitialVelocityJitter 0.4 0.4 0.4
			End
			LifeSpan 8
		End

		Event
			At Origin
			Type Start
			EName Sparks
			Part Emotes\Fireworks\Sparkle_Sparks_GREEN.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 9

		Event
			At SparkleAnchor
			Type Start
			Part Emotes\Fireworks\Sparkle_Flashes_Small_GREEN.part
			LifeSpan 30
		End

	End

End
