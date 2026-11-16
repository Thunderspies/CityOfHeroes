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
			PositionOffset		0 3 0
		End
		Part	:SeedIndicator_OuterRing_Green.part
		Part	:SeedIndicator_OuterRingTrail_Green.part
		Part	:SeedIndicator_Center_Green.part
		Part	:SeedIndicator_Center_Alpha_Green.part
		PArt	:SeedIndicator_Glow_Green.part
		#Part	:SeedIndicator_Trail.part
	End
End
#############################################################
End