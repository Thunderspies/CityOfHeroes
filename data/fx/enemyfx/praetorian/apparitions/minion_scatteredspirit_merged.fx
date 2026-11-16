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
	On 	time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_MinionAura_Loop 50 50 .35
	End


	Event
		Type	Local
		At	Root
		ChildFX	:Minion_Merge_Activation.fx
	End

	Event
		EName	SpinAnchor
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Spin		0.0 0.05 0.0
		End
		Flags	AdoptParentEntity
		Lifespan 1
	End

	Event
		Type	Local
		At	SpinAnchor
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
	#	Part	:Fog_Coastal_Strip_Lingering_Merged.part
		Part	:Fog_Coastal_Strip_Trail_Merged.part
	End

	Event
		Type	Local
		At	SpinAnchor
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
	#	Part	:Fog_Coastal_Strip_Lingering_Merged.part
		ChildFX	:Minion_ElectricAura.fx
	End
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 6.0 0.0
#		End
#		ChildFX	:Minion_ScatteredSpirit_BodyClouds_Merged.fx
#	End

End

Condition
	On	Cycle
	Time	10
	Chance 	.65

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Minion_ScatteredSpirit_Flash_Merged.fx
		Lifespan 5
	End
End

#############################################################

End