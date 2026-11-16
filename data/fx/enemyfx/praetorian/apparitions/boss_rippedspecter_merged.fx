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
		Sound 	Apparitions_BossAura_Loop 100 100 .5
	End

	Event
	#	EName	SpinAnchor
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		ChildFX	:Boss_Merge_Activation.fx
		#Lifespan 1
	End

	Event
		EName	SpinAnchor
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Spin		0.0 0.35 0.0
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
			Spin		0.0 0.2 0.0
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
		Part	:Dust_Ground_Circle_Merged.part
		Part	:Fog_Ground_Circle_Merged.part
		Part	:DustDevil_Dust_Core_Merged.part
	End

	Event
		Type	Local
		At	SpinAnchor
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
		Flags	AdoptParentEntity
		Part	:Dust_Ground_Spinning_Merged.part

	End

	Event
		Type	Local
		At	SpinAnchor2
		BhvrOverride
			PositionOffset	0.0 8.0 0.0
		End
		Part	:Dust_Ground_Spinning_Outer_Merged.part
	End

	Event
		Type	Local
		At	SpinAnchor3
		BhvrOverride
			PositionOffset	0.0 12.0 0.0
		End
		Part	:Dust_Thin_Rising_Merged.part
	End
End

Condition
	On	Cycle
	Time	10
	Chance 	.8

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Boss_RippedSpecter_Flash_Merged.fx
		Lifespan 5
	End
End

#############################################################

End