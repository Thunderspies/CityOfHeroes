#############################################################
## DualBlades - Light Opening Hit
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
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordHit2 70.0 70.0 .88
	End
End

#############################################################

Condition
	On	Time 
	Time	10
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound Swordstab5 70.0 70.0 .88
	End
End

#############################################################

End