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
		ENAME   p2
		Type	Local
		At	WepR
		Geom	FlameThrower
		Sound gatgun_short 120.0 120.0 1.0
		Lifespan	14
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan	12
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	11

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	17

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

#############################################################

End