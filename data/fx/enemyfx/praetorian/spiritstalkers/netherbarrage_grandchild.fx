#
FxInfo

	#LifeSpan 250


###########################################################

	Condition
		On Time
		Time 0

		Event
			ename anchor
			at Head
			type Start
			BhvrOverride
				PositionOffset 0 -1 0.7
				PyrRotate -10 0 0
			End
		End

		Event
			At anchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				InitialVelocity 	0 0 1
				InitialVelocityJitter	0.1 0.1 0.5
			End
			ChildFX :NetherBarrage_ProjectileChild.fx
			Lifespan 10
		End
	end

	Condition
		On Cycle
		Time 15

		Event
			At anchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				InitialVelocity 	0 0 1
				InitialVelocityJitter	0.1 0.1 0.5
				scale .5 .5 .5
			End
			ChildFX :NetherBarrage_ProjectileChild.fx
			Lifespan 10
		End



End
