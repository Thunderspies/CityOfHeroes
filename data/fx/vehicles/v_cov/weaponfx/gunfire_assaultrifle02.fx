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
	Time 	31

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	Arachnos_Pistol02
		Part1	WEAPONFX/Gunfire_AssaultShell.Part
		Sound Assault3 120.0 120.0 1.0
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Geom	FX_MachineGunBurst
		LifeSpan	34
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
		LifeSpan	35
	End

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	35
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
		LifeSpan	38
	End

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	38
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
		LifeSpan	41
	End

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	41
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
		LifeSpan	44
	End

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		LifeSpan	44
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
		Ename 	All
		Type	Destroy
	End
End

End				