#############################################################
## DualBlades - Special 1 Hit
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
		Sound SwordSlicer 70.0 70.0 .68
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
		Sound SwordStab3 70.0 70.0 .68
	End
End

#############################################################

Condition
	On	Time 
	Time	25
	
	Event
		Type 	Local
		At	Chest
		BHVR	Behaviors/CameraShake01Subtle.bhvr
		ChildFX	:DualBlades_HitBig1.fx
		Sound SwordStab4 70.0 70.0 .68
	End
End

#############################################################

Condition
	On	Time 
	Time	45
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlades_HitSmall1.fx
		Sound SwordStab2 70.0 70.0 .68
	End
End

#############################################################

End