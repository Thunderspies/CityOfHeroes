#############################################################
## DualBlades - Moderate Opening Attack (Initial)
#############################################################

FxInfo

LifeSpan 105

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Origin
		ChildFX	:DualBlades_ModerateOpening_Attack.fx
	End
End

#############################################################

End