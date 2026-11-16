#############################################################
## DualBlades - LightOpening Hit
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitBig1.fx
		Sound clawswipe5 70.0 70.0 .88
	End
End

#############################################################

End