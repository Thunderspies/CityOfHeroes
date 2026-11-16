#########################################################
##	
##

FxInfo
LifeSpan 80 
Flags	InheritAnimScale

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	C_Fore_ToeR
End
#
##################################

Condition
	On 	Time
	Time 	26

	Event
		ENAME   p1
		Type	Start 
		At	C_Fore_ToeR
		#Bhvr	:Arachnos_GunFlash.bhvr
		#Geom	FlameThrower
		#Geom	TESTING_Target
		Sound Assault3 120.0 120.0 1.0
	End

	Event
		EName 	Blast1
		Type	Start
		At	p1
		Bhvr	:Arachnos_GunFlash.bhvr
#		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
	End

	Event
		EName 	Flare1
		Type	Start
		At	Blast1
		#Altpiv 1
		#Bhvr	:Arachnos_GunFlash.bhvr
		Part1	:Arachnos_TorchflameA.part
		Part2	:Arachnos_TorchflameB.part
	End

End

##################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Blast2
		Type	Start
		At	C_Fore_ToeR
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
	End

End

#################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Blast3
		Type	Start
		At	C_Fore_ToeR
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
	End

End

#################################################

Condition
	On 	Time
	Time 	35

	Event
		EName 	Blast4
		Type	Start
		At	C_Fore_ToeR
		Bhvr	:Arachnos_GunFlash.bhvr
		Geom	FX_MachineGunBurst
		Part1	:Arachnos_GunFlash.part
		Part2	:Arachnos_GunGlow.part
	End

End

#################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Smoke
		Type	Local
		At	C_Fore_ToeR
		
		
		Part	:Crab_GunSmoke01.part
	End

End


##########  Killing  ###########

Condition
	On 	Time
	Time	33

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	36

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	38

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	40

	Event
		Ename 	Blast4
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	80

	Event
		Ename 	Smoke
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	40

	Event
		Ename 	Flare1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	60

	Event
		Ename 	All
		Type	Destroy
	End
End

End				