#############################################################
## Child_RandomSteamJets1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Col_L
		BhvrOverride
			PositionOffset		-0.375 0.5 -0.375
			PyrRotate		30 0 -15
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Col_R
		BhvrOverride
			PositionOffset		0.375 0.5 -0.375
			PyrRotate		30 0 15
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	UArmL
		BhvrOverride
			PositionOffset		-0.625 0.0 -0.75
			PyrRotate		85 0 -10
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	UArmR
		BhvrOverride
			PositionOffset		0.625 0.0 -0.75
			PyrRotate		85 0 15
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	ULegL
		BhvrOverride
			PositionOffset		-0.375 -1.25 0.25
			PyrRotate		0 15 -85
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	ULegR
		BhvrOverride
			PositionOffset		0.375 -1.25 0.25
			PyrRotate		0 -15 85
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	LLegL
		BhvrOverride
			PositionOffset		-0.375 -1.5 -0.625
			PyrRotate		85 0 -45
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	LLegR
		BhvrOverride
			PositionOffset		0.375 -1.5 -0.625
			PyrRotate		85 0 45
		End
		ChildFX	:Child_SingleSteamJet.fx
	End
End

#############################################################

End