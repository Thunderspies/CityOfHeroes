#########################################################
## Vanguard "Redding" Rifle - Burst (Initial Delayed)
#########################################################

FxInfo

LifeSpan  100

#########################################################

Condition
	On 	Time
	Time 	41

	Event
		ENAME   AltPivots
		Type	Local 
		At	WepR
		Geom	Vanguard_Rifle
		Sound Mercenary_machgunBurst 120.0 120.0 1.0
		Lifespan	14
	End

	Event
		EName 	Blast1
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan	2
	End

	Event
		EName 	tracer
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		#Part3	WEAPONFX/TorchsparksA.part
		#Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		#Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		#Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
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
	Time 	42

	Event
		EName 	Blast2
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
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
	Time 	45

	Event
		EName 	Blast3
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
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
	Time 	48

	Event
		EName 	Blast4
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
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
	Time 	51

	Event
		EName 	Blast5
		Type	local
		At	AltPivots
		Altpiv	1
		BHVR	WEAPONs\WeaponRotate.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Lifespan	2
	End

	Event
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End
End

#########################################################

End