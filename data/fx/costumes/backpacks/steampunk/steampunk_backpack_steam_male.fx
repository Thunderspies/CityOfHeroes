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
			PositionOffset	0 0.5 -0.4
		End
		GEOM	FX_GEO_SteamPack_Offsets
	End

	Event
		EName 	SputterR
		Type	Local
		At	PackOffsets
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 -0.2 -0.8
		End
		Part	:SteamJets_Sputter_Sideways.part
		Part	:SteamJets_Jets_Sideways.part

	End
End

Condition
	On 	Cycle
	Time 	12
	Random	1
	Chance	0.4

	Event
		EName 	SputterR
		Type	Local
		At	PackOffsets
		AltPiv	2
		BhvrOverride
			PositionOffset	1.15 1.5 -0.45
		End
		Part	:SteamJets_Sputter.part
		Part	:SteamJets_Jets.part
		Lifespan	7
		LifespanJitter	3
	End
End


#############################################################

End