#
FxInfo
Lifespan 100


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
			Part :Bloom_Glow.part
			Part :Bloom_Smoke.part
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
			Part :Bloom_Tendril.part
			Part :Bloom_Sparks.part
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
			Part :Bloom_Tendril.part
			LifeSpan 11
		End

	End

End
