#############################################################
## DualBlades - Special 1 Attack (Initial)
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
		ChildFX	:DualBlades_Special1_Attack.fx
	End
End

#############################################################

End