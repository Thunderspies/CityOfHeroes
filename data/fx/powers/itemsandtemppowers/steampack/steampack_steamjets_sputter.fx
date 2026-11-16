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
		SoundNoRepeat 2
		Sound 	SP_SteamPack_Sputter_01 100 100 .2
		Sound 	SP_SteamPack_Sputter_02 100 100 .1
		Sound 	SP_SteamPack_Sputter_03 100 100 .15
		Sound 	SP_SteamPack_Sputter_04 100 100 .07
		Sound 	SP_SteamPack_Sputter_05 100 100 .175
		Lifespan	7
		LifespanJitter	3

	End

	Event
		EName 	SputterR
		Type	Local
		At	PackOffsets
		AltPiv	2
		Part	:SteamJets_Sputter.part
		SoundNoRepeat 2
		Sound 	SP_SteamPack_Sputter_01 100 100 .1
		Sound 	SP_SteamPack_Sputter_02 100 100 .175
		Sound 	SP_SteamPack_Sputter_03 100 100 .07
		Sound 	SP_SteamPack_Sputter_04 100 100 .2
		Sound 	SP_SteamPack_Sputter_05 100 100 .15
		Lifespan	7
		LifespanJitter	3
	End
End

#############################################################

End