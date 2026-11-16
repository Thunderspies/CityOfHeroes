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
		Geom	Arachnos_Pistol02
		Sound MachGunBurst_short 120.0 120.0 1.0
		Lifespan	14
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Geom	FX_MachineGunBurst
		LifeSpan	14
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	local
		At	p2
		Altpiv	1
		Geom	FX_MachineGunBurst
		LifeSpan	2
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	local
		At	p2
		Altpiv	1
		Geom	FX_MachineGunBurst
		LifeSpan	2
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	p2
		Altpiv	1
		Geom	FX_MachineGunBurst
		LifeSpan	2
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	local
		At	p2
		Altpiv	1
		Geom	FX_MachineGunBurst
		LifeSpan	2
	End

	Event
		Type	local
		At	p2
		Altpiv	1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	2
	End

	Event
		Type	Local
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	1
	End
End

#############################################################

End