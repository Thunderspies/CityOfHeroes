#############################################################
## DualBlades - Special 2 Attack (Initial)
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
		ChildFX	:DualBlades_Special2_Attack.fx
	End
End

#############################################################

End