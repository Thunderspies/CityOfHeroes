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
		Sound 	Apparitions_MinionAura_Loop 75 75 .25
	End

#	Event
#		Type	Local
#		At	Root
#		Bhvr	behaviors\fadeOut.bhvr
#		BhvrOverride
#			PositionOffset	0.0 1.0 0.0
#		End
#		Flags	AdoptParentEntity
#
#	#	Part	:Fog_Coastal_Strip.part
#		Lifespan 1
#	End
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 2.0 0.0
#		End
#	#	Part	:Fog_Coastal_Strip_Lingering.part
#	End
#
#	Event
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 3.0 0.0
#		End
#	#	Part	:Fog_Coastal_Strip_Lingering.part
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
		ChildFX	:Minion_ScatteredSpirit_Flash.fx
		Lifespan 5
	End
End

#############################################################

End