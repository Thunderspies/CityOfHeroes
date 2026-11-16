#
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 1

		Event
			At Head
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :PsioHeadRings_In_01.part
			Part :PsioHeadRings_In_02.part
			Part :PsioHeadGlow.part
			Part :EnergyCoreFlash.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			Lifespan 30
		End
	End



End
