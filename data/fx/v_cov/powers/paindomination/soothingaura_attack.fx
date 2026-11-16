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
		Bhvr	behaviors\GenericParticleFade.bhvr
		#Part4	:SoothingAura_PulseRing01.part
		Part4	:SoothingAura_PulseRing02.part
		LifeSpan 5
	End
End

#############################################################

End