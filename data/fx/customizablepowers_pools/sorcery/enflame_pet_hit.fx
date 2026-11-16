#
FxInfo

Lifespan 30

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0
			End
			Part :FireHit_Core.part
			Part :FireHit_Highlight.part
			Part :FireHit_Smoke.part
		End

	End

End
