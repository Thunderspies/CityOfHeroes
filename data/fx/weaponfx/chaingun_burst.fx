#########################################################
##	
##

FxInfo
LifeSpan  105

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	31

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	FlameThrower
		Bhvr	Behaviors/ChainGunBustMedium.bhvr
		#Part1	:Gunfire_AssaultShell.Part
		Sound chaingun_short2 120.0 120.0 1.0
	End

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		#Part3	WEAPONFX/TorchsparksA.part
		#Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		#Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		#Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		
		Lifespan	14
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

End


Condition
	On 	Time
	Time 	35

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

End

Condition
	On 	Time
	Time 	38

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

End

Condition
	On 	Time
	Time 	41

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

End


Condition
	On 	Time
	Time 	44

	Event
		ENAME   eject
		Type	Local 
		At	p2
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	60
	End

End



##################################

Condition
	On 	Time
	Time	34

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	37

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	40

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	Blast4
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	45

	Event
		Ename 	Blast5
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	45

	Event
		Ename 	tracer
		Type	Destroy
	End
End

End				