#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Head
		Part	:Hands_Mist.part
		Part	:Head_LightRays_Small.part
		Part	:Hands_Rings.part
		Lifespan 50
	End
End

##############################################################################

End