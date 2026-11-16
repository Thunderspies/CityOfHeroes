#############################################################
## DualBlades - Light Opening Attack (Initial)
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
		ChildFX	:DualBlades_LightOpening_Attack.fx
	End
End

#############################################################

End