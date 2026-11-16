#############################################################
## Assault Rifle - Full Auto Attack FX
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	local
		At	C_Chest
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part
		Sound FullAuto 120.0 120.0 0.9
		Lifespan 112
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WepR
		ChildFX	:Player_AssaultRifle_CycleShot_Child.fx
		Lifespan 112
	End
End

#############################################################

End