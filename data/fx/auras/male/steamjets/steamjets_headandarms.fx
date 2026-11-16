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
		At	LArmL
		BhvrOverride
			PositionOffset		0.0 0.15 -0.1
			PyrRotate		0 50 40
		End
		ChildFX	:Child_SteamJet.fx
	End

	Event
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset		0.0 0.15 -0.1
			PyrRotate		0 -50 -40
		End
		ChildFX	:Child_SteamJet.fx
	End

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset		0.0 0.1 0.0
			PyrRotate		20 -30 -10
		End
		ChildFX	:Child_SteamJet_Small.fx
	End

	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset		0.0 0.1 0.0
			PyrRotate		20 30 10
		End
		ChildFX	:Child_SteamJet_Small.fx
	End
End

#############################################################

End