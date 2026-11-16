#############################################################
## Child_RandomSteamJets1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	35
	Chance	0.2

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.3 -1.25 0.25
			PyrRotate		0 -45 -55
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan	8
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	0.2

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.3 -1.25 0.25
			PyrRotate		0 45 55
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan	8
	End
End

Condition
	On 	Cycle
	Time 	18
	Chance	0.15

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.1 -1.5 -0.25
			PyrRotate		0 -50 -70
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan	8
	End
End

Condition
	On 	Cycle
	Time 	18
	Chance	0.15

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.1 -1.5 -0.25
			PyrRotate		0 50 70
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan	8
	End
End

#############################################################

End