#############################################################
## DualBlades - AOE Bridge Attack (Initial)
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Origin
		ChildFX	:DualBlades_AOEBridge_Attack.fx
	End
End

#############################################################

End