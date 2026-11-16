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
	Time 	41

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	Arachnos_Pistol02
		Part1	WEAPONFX/Gunfire_AssaultShell2.Part
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
		LifeSpan	44
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Lifespan	47
	End

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		Lifespan	47
	End
End


Condition
	On 	Time
	Time 	45

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Lifespan	50
	End

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		Lifespan	50
	End

End

Condition
	On 	Time
	Time 	48

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Lifespan	53
	End

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		Lifespan	53
	End

End

Condition
	On 	Time
	Time 	51

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Lifespan	56
	End

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		BHVR	:PistolOffset2.bhvr
		Part1	WEAPONFX/MachineGunFlash.part
		Part2	WEAPONFX/MachineGunGlow.part
		Lifespan	56
	End

End




##################################

Condition
	On 	Time
	Time	44

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	47

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	50

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	53

	Event
		Ename 	Blast4
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	55

	Event
		Ename 	All
		Type	Destroy
	End
End

End				