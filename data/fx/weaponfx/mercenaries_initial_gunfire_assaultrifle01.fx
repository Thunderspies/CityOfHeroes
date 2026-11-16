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
		Geom	Mastermind_Military_Rifle
		#Part1	:Gunfire_AssaultShell2.Part
		Sound SCARBurst2 120.0 120.0 .6
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX/TorchflameBlue1.part
		Part2	WEAPONFX/TorchflameBlue2.part
		#Part3	WEAPONFX/TorchsparksA.part
		#Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		#Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		#Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
	End

End


Condition
	On 	Time
	Time 	45

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
	End

End

Condition
	On 	Time
	Time 	48

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
	End

End

Condition
	On 	Time
	Time 	51

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
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