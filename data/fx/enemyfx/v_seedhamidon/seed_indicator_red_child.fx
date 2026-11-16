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
		Part	:SeedIndicator_OuterRing_Red.part
		Part	:SeedIndicator_OuterRingTrail_Red.part
		Part	:SeedIndicator_Center_Red.part
		Part	:SeedIndicator_Center_Alpha_Red.part
		PArt	:SeedIndicator_Glow_Red.part
		#Part	:SeedIndicator_Trail.part
	End
End
#############################################################
End