#############################################################
## DualBlades - Moderate Opening Hit
#############################################################

FxInfo

LifeSpan 5

LifeSpan 150

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordHit3 70.0 70.0 .88
	End
End

#############################################################

Condition
	On	Time 
	Time	12
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab5 70.0 70.0 .88
	End
End

#############################################################

End