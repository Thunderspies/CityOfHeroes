#############################################################
## SteamPack_SteamJets.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	SP_SteamPack_Loop 100 100 .45
	End

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
	On	TriggerBits
	TriggerBits	JUMP
	DoMany		1

	Event
		EName 	Prime
		Type	Local
		At	PackOffsets
		AltPiv	1
		While	JUMP
		Flags	OneAtATime
		Part	:SteamJets.part
		Part	:SteamJets_Initial.part
	End

	Event
		EName 	Prime2
		Type	Local
		At	PackOffsets
		AltPiv	2
		While	JUMP
		Flags	OneAtATime
		Part	:SteamJets.part
		Part	:SteamJets_Initial.part
	End
End

#############################################################

End