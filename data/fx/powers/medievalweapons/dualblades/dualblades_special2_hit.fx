#############################################################
## DualBlades - Special 2 Hit
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
		BHVR	Behaviors/CameraShake01Subtle.bhvr
		ChildFX	:DualBlades_HitBig1.fx
	End
End

#############################################################

End