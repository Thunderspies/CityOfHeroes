###########################################################

FxInfo

	Flags DontSuppress

###########################################################
	Condition
		On	Time
		Time	0

		Event
			Ename HeadAnchor
			At Chest
			Type Local
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.5 -0.6
			End
			Part	:Volcano_Smoke.part
		End
	End
	Condition
		On 	Cycle
		Time 	20
		Chance	0.5

		Event
			Ename HeadAnchor
			At Chest
			Type Local
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.5 -0.6
			End
			Part	:Volcano_Smoke.part
			Part	:Sparks.part
			Lifespan 1
		End

		Event
			Ename HeadAnchor2
			At Chest
			Type Local
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PYRRotate	0.0 0.0 45
				PositionOffset 0 0.3 -0.6
			End
			Part	:Fire_Anim.part
			Part	:Fire_Anim_Additive.part

			Lifespan 3
			Lifespan 2
		End
	End

###########################################################

End
