#############################################################
## DualBlades - Follow Up Attack (Initial)
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
		ChildFX	:DualBlades_FollowUp_Attack.fx
	End
End

#############################################################

End