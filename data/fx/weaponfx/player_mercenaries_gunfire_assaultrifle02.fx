#############################################################
## Assault Rifle - Burst Attack FX
#############################################################

FxInfo

LifeSpan  30

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WepR
		Sound Mercenary_machgunBurst 120.0 120.0 1.0
		LifeSpan	14
	End

	Event
		EName 	tracer
		Type	local
		At	C_Chest
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan	12
	End

	Event
		Type	Local
		At	WepR
		ChildFX	:Player_AssaultRifle_CycleShot_Child.fx
		Lifespan	12
	End
End

#############################################################

End