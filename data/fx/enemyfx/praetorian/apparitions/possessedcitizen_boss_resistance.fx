#############################################################
## Snow_Fists.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		EName	MouthAnchor
		Type	Local
		At	head
		bhvr	behaviors\scale1a.bhvr
		Geom	LocalPivot01
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Body_SmokeAura.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Body_SmokeAura.part
	End
End

Condition
	On	Cycle
	Time	10
	Chance 	.35

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:PossessedCitizen_BodyGlow.fx
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	MouthAnchor
		Altpiv	4
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 .5
		End
		Part	:Mouth_Breath.part
		Lifespan	15
	End
End

#########################################################

End