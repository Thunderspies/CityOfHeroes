#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_LieutenantAura_Loop 100 100 .5
	End
	Event
		EName	SpinAnchor
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Spin		0.0 0.2 0.0
		End
		Flags	AdoptParentEntity
		Lifespan 1
	End

	Event
		EName	SpinAnchor2
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Spin		0.0 0.1 0.0
		End
		Flags	AdoptParentEntity
		Lifespan 1
	End

	Event
		EName	SpinAnchor3
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Spin		0.0 0.05 0.0
		End
		Flags	AdoptParentEntity
		Lifespan 1
	End

#############################################################

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Flags	AdoptParentEntity
		Part	:Fog_Ground_Circle.part
	End

	Event
		Type	Local
		At	SpinAnchor
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
		Flags	AdoptParentEntity
		Part	:Fog_Coastal_Strip_Spinning_Quick.part
		Part	:Fog_Coastal_Core.part

	End

	Event
		Type	Local
		At	SpinAnchor2
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
		Part	:Fog_Coastal_Strip_Spinning.part
	End

	Event
		Type	Local
		At	SpinAnchor3
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
		Part	:Fog_Coastal_Strip_Thin_Spinning.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
	#	ChildFX	:Lieutenant_DispersedImpression_BodyClouds.fx
	End
End

Condition
	On	Cycle
	Time	10
	Chance 	.3

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Lieutenant_DispersedImpression_Flash.fx
		Lifespan 5
	End
End

Condition
	On	DEATH

	Event
		EName	All
		Type	Destroy
	End

	Event
		Type	Local
		At	Root
		ChildFX	:Lieutenant_DispesedImpression_Death.fx
		Lifespan 30
	End
End

#############################################################

End