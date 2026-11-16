#############################################################
## Child_RandomSteamJets1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	30
	Chance	0.175

	Event
		Type	Local
		At	Col_L
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.35 0.0
			PyrRotate		30 0 -25
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	40
	Chance	0.25

	Event
		Type	Local
		At	Col_R
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.35 0.0
			PyrRotate		30 0 25
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End


Condition
	On 	Cycle
	Time 	40
	Chance	0.25

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.15 -0.1
			PyrRotate		0 50 40
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	30
	Chance	0.175
	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.15 -0.1
			PyrRotate		0 -50 -40
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	20
	Chance	0.125

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.1 0.0
			PyrRotate		20 -30 -10
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	20
	Chance	0.125


	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.1 0.0
			PyrRotate		20 30 10
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	0.15

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.3 -1.25 0.25
			PyrRotate		0 -45 -55
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	0.15

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.3 -1.25 0.25
			PyrRotate		0 45 55
		End
		ChildFX	:Child_SteamJet.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	18
	Chance	0.1

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.1 -1.5 -0.25
			PyrRotate		0 -50 -70
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End

Condition
	On 	Cycle
	Time 	18
	Chance	0.1

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.1 -1.5 -0.25
			PyrRotate		0 50 70
		End
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 8
	End
End

#############################################################

End