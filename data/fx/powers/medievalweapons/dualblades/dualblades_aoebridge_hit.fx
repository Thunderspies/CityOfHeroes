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
		Sound SwordSlicer 70.0 70.0 .88
	End
End

#############################################################

Condition
	On	Time 
	Time	10
	
	Event
		Type 	Local
		At	Chest
		Sound swordstab1 70.0 70.0 .5
	End
End

Condition
	On	Time 
	Time	15
	
	Event
		Type 	Local
		At	Chest
		Sound swordstab2 70.0 70.0 .6
	End
End

Condition
	On	Time 
	Time	20
	
	Event
		Type 	Local
		At	Chest
		Sound swordstab3 70.0 70.0 .7
	End
End

Condition
	On	Time 
	Time	25
	
	Event
		Type 	Local
		At	Chest
		Sound swordstab4 70.0 70.0 .8
	End
End

Condition
	On	Time 
	Time	30
	
	Event
		Type 	Local
		At	Chest
		Sound swordstab1 70.0 70.0 .8
	End
End

#############################################################

End