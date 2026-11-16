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
			Part :PsioHeadRings_Confuse_01.part
			Part :PsioHeadRings_Confuse_02.part
			Part :PsioHeadGlow.part
			Part :EnergyCoreFlash.part
			Part CustomizeablePowers\PsionicAssault\InfuseLight2.part
			#Lifespan 30
		End

		Event
			Type	local
			At	Head
			BHVR	Behaviors\GenericParticleFade.bhvr
			Part	:PercentSign.part
			Part	:Asterick.part
			Part	:QuestionMark.part
			Part	:NumberSign.part
			Part	:PsionicBubblesHit.part
		End

	End



End
