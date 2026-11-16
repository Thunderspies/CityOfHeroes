#############################################################
## Child_RandomSteamJets1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	40
	Chance	0.25

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.1 -0.1
			PyrRotate		0 50 40
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan	8
	End
End

Condition
	On 	Cycle
	Time 	20
	Chance	0.2

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.1 0.0
			PyrRotate		20 -30 -10
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan	8
	End
End


#############################################################

End