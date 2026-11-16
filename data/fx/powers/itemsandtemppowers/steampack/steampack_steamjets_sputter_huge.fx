#############################################################
## SteamPack_SteamJets_Sputter.fx
#############################################################

FxInfo
LifeSpan 0

Flags	InheritGeoScale

#############################################################

Input
	InpName	Hips
End

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName	PackOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Scale		1.4 1.4 1.4
			PositionOffset	0 0.6 -0.8
		End
		GEOM	FX_GEO_SteamPack_Offsets
	End
End

Condition
	On 	Cycle
	Time 	12
	Random	1
	Chance	0.4

	Event
		EName 	SputterL
		Type	Local
		At	PackOffsets
		AltPiv	1
		Part	:SteamJets_Sputter.part
		Lifespan	7
		LifespanJitter	3

	End

	Event
		EName 	SputterR
		Type	Local
		At	PackOffsets
		AltPiv	2
		Part	:SteamJets_Sputter.part
		Lifespan	7
		LifespanJitter	3
	End
End

#############################################################

End