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
		Sound clawswipe5 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	5
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab3 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	10
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordSlicer 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	15
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordHit3 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	20
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		#Sound 	SwordStab2 70.0 30.0 .88	
	End
End

Condition
	On	Time 
	Time	25
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab1 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	30
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordHit3 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	35
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab5 70.0 70.0 .88
	End
End

Condition
	On	Time 
	Time	40
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		#Sound 	SwordStab4 70.0 30.0 .88	
	End
End

Condition
	On	Time 
	Time	45
	
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
	Time	68
	
	Event
		Type 	Local
		At	Chest
		BHVR	Behaviors/CameraShake01Subtle.bhvr
		ChildFX	:DualBlades_HitBig1.fx
	End
End

#############################################################

End