#############################################################
## Arachnos Soldier - Wolf Spider - Burst
#############################################################

FxInfo

LifeSpan 20

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	C_Chest
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part
		Geom	FX_MachineGunBurst
		Sound MachGunBurst_short 120.0 120.0 1.0
		LifeSpan 13
	End

	Event
		Type	Local
		At	WepR
		ChildFX	:Child_MuzzleFlash_RapidFire.fx
		LifeSpan 13
	End
End

#############################################################

End