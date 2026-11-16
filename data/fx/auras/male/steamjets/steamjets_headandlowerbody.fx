#############################################################
## SteamJets_UpperBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		BhvrOverride
			PositionOffset		-0.2 -0.05 -0.25
			PyrRotate		0 0 -90
		End
		ChildFX	:Child_SteamJet_Small.fx
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset		0.2 -0.05 -0.25
			PyrRotate		0 0 90
		End
		ChildFX	:Child_SteamJet_Small.fx
	End

#############################################################

	Event
		Type	Local
		At	ULegL
		BhvrOverride
			PositionOffset		-0.3 -1.25 0.25
			PyrRotate		0 -45 -55
		End
		ChildFX	:Child_SteamJet.fx
	End

	Event
		Type	Local
		At	ULegR
		BhvrOverride
			PositionOffset		0.3 -1.25 0.25
			PyrRotate		0 45 55
		End
		ChildFX	:Child_SteamJet.fx
	End

	Event
		Type	Local
		At	LLegL
		BhvrOverride
			PositionOffset		-0.1 -1.5 -0.25
			PyrRotate		0 -50 -70
		End
		ChildFX	:Child_SteamJet_Small.fx
	End

	Event
		Type	Local
		At	LLegR
		BhvrOverride
			PositionOffset		0.1 -1.5 -0.25
			PyrRotate		0 50 70
		End
		ChildFX	:Child_SteamJet_Small.fx
	End

#############################################################

End