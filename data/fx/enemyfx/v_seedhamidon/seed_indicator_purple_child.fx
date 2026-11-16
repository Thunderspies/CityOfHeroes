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
		BhvrOverride
			PositionOffset		0 2 0
		End
		Part	:SeedIndicator_OuterRing_Purple.part
		Part	:SeedIndicator_OuterRingTrail_Purple.part
		Part	:SeedIndicator_Center_Purple.part
		Part	:SeedIndicator_Center_Alpha_Purple.part
		PArt	:SeedIndicator_Glow_Purple.part
		#Part	:SeedIndicator_Trail.part
	End
End
#############################################################
End