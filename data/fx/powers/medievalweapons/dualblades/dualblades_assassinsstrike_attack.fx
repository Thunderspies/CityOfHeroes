#############################################################
## DualBlades - Assassins Strike
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlade_HitSmall1.fx
	End
End

#############################################################

Condition
	On	Time 
	Time	15
	
	Event
		Type 	Local
		At	Chest
		ChildFX	:DualBlade_HitSmall1.fx
	End
End

#############################################################

End