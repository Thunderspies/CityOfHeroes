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
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab3 70.0 70.0 .88
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
		Sound SwordSlicer 60 60 .76
	End
End

#############################################################

Condition
	On	Time 
	Time	21
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordHit2 60 60 .76
	End
End

#############################################################

End