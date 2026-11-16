#############################################################
## Radiant Aura
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\PainDomination\SoothingAura_PulseRing02.part
		LifeSpan 5
	End
End

#############################################################

End