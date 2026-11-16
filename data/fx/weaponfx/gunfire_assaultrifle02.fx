############################################################
## Header
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   WeaponOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
		Sound Assault3 120.0 120.0 1.0
		Lifespan	14
	End

	Event
		Type	local
		At	WeaponOffset
		Altpiv	1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan	12
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
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
	Time 	10

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
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
	Time 	13

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
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
	Time 	16

	Event
		Type	Local
		At	WeaponOffset
		Altpiv	1
		Bhvr	Behaviors/MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
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