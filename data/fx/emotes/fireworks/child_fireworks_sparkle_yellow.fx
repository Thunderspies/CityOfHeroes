#
FxInfo
LIfespan 40


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			Part :Bloom_Flash.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type Start
			Part :Bloom_Flash_Glow.part
			Part Emotes\Fireworks\Sparkle_Glow.part
			Part :Bloom_Smoke.part
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
			Part Emotes\Fireworks\Sparkle_Sparks.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 9

		Event
			At SparkleAnchor
			Type Start
			Part Emotes\Fireworks\Sparkle_Flashes_Small.part
			LifeSpan 30
		End

	End

End
