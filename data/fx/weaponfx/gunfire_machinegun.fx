#########################################################
##	
##

FxInfo
LifeSpan  100

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
	Time 	30

	Event
		Ename	Shells
		Type	local
		At	WepR
		#Part1	WEAPONFX/Gunfire_AssaultShell.Part
	End

End

Condition
	On 	Time
	Time 	29

	
	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	gernadelauncher
		lifespan	16
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
	Time 	33

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
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

End


Condition
	On 	Time
	Time 	36

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
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

End

Condition
	On 	Time
	Time 	39

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
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
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
	Time	42

	Event
		Ename 	Blast4
		Type	Destroy
	End
End



End				