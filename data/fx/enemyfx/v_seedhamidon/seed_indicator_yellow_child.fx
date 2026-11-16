#############################################################
## Seed_Indicator.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 2 0
		End
		Part	:SeedIndicator_OuterRing_Yellow.part
		Part	:SeedIndicator_OuterRingTrail_Yellow.part
		Part	:SeedIndicator_Center_Yellow.part
		Part	:SeedIndicator_Center_Alpha_Yellow.part
		PArt	:SeedIndicator_Glow_Yellow.part
	End
End
#############################################################
End