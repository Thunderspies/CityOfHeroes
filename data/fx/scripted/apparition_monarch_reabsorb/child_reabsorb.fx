#############################################################
## Apparition_Monarch_Reabsorb.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:CrushingHate_Border.part
		Part	:CrushingHate_Swirls.part
		Lifespan 20
	End

End

#########################################################

End