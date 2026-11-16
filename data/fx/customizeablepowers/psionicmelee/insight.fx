#
FxInfo

	LifeSpan 30


###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename BottomAnchor
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.5 0
				InitialVelocity 0 0.15 0
			End
			Lookat TopAnchor
			Pother TopAnchor
		End

		Event
			Ename AllSeeingEye
			At BottomAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Insight_Eye.part
			Part :Insight_Streaks.part
			Part :Insight_Glow.part
			Part :Insight_Glow_Add.part
			Lifespan 5
		End

		Event
			Ename AllSeeingEye
			At BottomAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Insight_Pupil.part
		End
	End



End
