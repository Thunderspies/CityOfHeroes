#
FxInfo

Lifespan 200

###########################################################

	Condition
		On Time
		Time 0

		Event
			At UArmL
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.3 0
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Core.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Highlight.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_black.part
		End

		Event
			At UArmR
			Type Local
			POther Neck
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 -0.3 0
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Core.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Highlight.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_black.part
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0
			End
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Core.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_Highlight.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Plasma_black.part
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.7 0
			End
			Part IncarnatePowers\Interface\Plasma_CoreA.part
		End

	End

End
