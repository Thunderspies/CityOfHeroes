#############################################################
## SelfDestruction_Windup.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0
	Chance	0.25

	Event
		Type	Local
		At	Root
		ChildFX	POWERS\ItemsAndTempPowers\SelfDestruction\Child_ElectricalBodySurge1.fx
		Lifespan 60
	End
End

Condition
	On	Cycle
	Time	45
	Chance	0.25

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\ItemsAndTempPowers\SelfDestruction\Child_ElectricalBodySurge1.fx
		Lifespan 60
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		ChildFX	POWERS\ItemsAndTempPowers\SelfDestruction\Child_RandomSparks1.fx
	End
End

#############################################################

End