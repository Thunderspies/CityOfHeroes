#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			Part Emotes\Fireworks\Bloom_Flash_Red.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type Start
			Part Emotes\Fireworks\Bloom_Flash_Glow_Red.part
			Part Emotes\Fireworks\Bloom_Glow_RED.part
			Part Emotes\Fireworks\Bloom_Smoke_Red.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 3
		Repeat 8

		Event
			At Origin
			Type Start
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Drag 0.05
				Gravity 0.0015
				InitialVelocityJitter 0.4 0.4 0.4
			End
			Part Emotes\Fireworks\Bloom_Tendril_RED.part
			Part Emotes\Fireworks\Bloom_Sparks_RED.part
			LifeSpan 13
			LifeSpanJitter 1
		End

	End

	Condition
		On Time
		Time 5
		Repeat 20

		Event
			At Origin
			Type Start
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				InitialVelocityJitter 0.6 0.6 0.6
				Gravity .001
				Drag 0.05
			End
			Part Emotes\Fireworks\Bloom_Tendril_RED.part
			LifeSpan 11
		End

	End

End
