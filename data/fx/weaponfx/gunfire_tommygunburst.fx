#############################################################
## Header
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   Offset1
		Type	Local
		At	WepR
		Geom	TommyGunWepR
		Sound tommyGunShort 120.0 120.0 1.0
		Lifespan	14
	End

	Event
		Type	local
		At	Offset1
		Altpiv	1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan	12
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	local
		At	Offset1
		Altpiv	1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	local
		At	Offset1
		Altpiv	1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Offset1
		Altpiv	1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	local
		At	Offset1
		Altpiv	1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

#############################################################

End