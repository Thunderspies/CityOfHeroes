#############################################################
## Arachnos Soldier - Wolf Spider - Heavy Burst
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WepR
		Sound machgunBurst 120.0 120.0 .9
	End

	Event
		Type	Local
		At	C_Chest
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part
		LifeSpan 44
	End
End

#############################################################

Condition
	On 	Time
	Time 	4 #Starts 3 frames earier because of Cycle delay

	Event
		Type	Local
		At	WepR
		ChildFX	:Child_MuzzleFlash_RapidFire.fx
		Lifespan 47
	End
End

#############################################################

End