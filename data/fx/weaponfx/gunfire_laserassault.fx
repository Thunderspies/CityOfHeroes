#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End 

Flags    InheritAlpha

##################################
Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime
		Type	start 
		At	WepL
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyBullet.part
		
		Magnet	Target
		LookAt	Target
		
	End

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	LaserAssault
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		#Geom	FX_MachineGunBurst
		Part1	:EnergyGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/EnergyflameA.part
		Part2	WEAPONFX/EnergyflameB.part
		
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
		#Geom	FX_MachineGunBurst
		Part1	:EnergyGunFlash.part
		Part2	:MachineGunGlow.part
	End
	
	Event
		EName 	Prime2
		Type	start 
		At	WepL
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyBullet.part
		
		Magnet	Target
		LookAt	Target
		
		
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
		#Geom	FX_MachineGunBurst
		Part1	:EnergyGunFlash.part
		Part2	:MachineGunGlow.part
	End
	
	Event
		EName 	Prime3
		Type	start 
		At	WepL
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyBullet.part
		
		Magnet	Target
		LookAt	Target
		
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
		#Geom	FX_MachineGunBurst
		Part1	:EnergyGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Prime4
		Type	start 
		At	WepL
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyBullet.part
		
		Magnet	Target
		LookAt	Target
		
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
		#Geom	FX_MachineGunBurst
		Part1	:EnergyGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	Prime5
		Type	start 
		At	WepL
		Bhvr	Behaviors\Fastprojectile.bhvr
		Part1	:EnergyBullet.part
		
		Magnet	Target
		LookAt	Target
		
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
	Time	58
	
	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	Prime2
		Type	Destroy
	End

	Event
		Ename 	Prime3
		Type	Destroy
	End

	Event
		Ename 	Prime4
		Type	Destroy
	End

	Event
		Ename 	Prime5
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	38.5

	Event
		Ename 	tracer
		Type	Destroy
	End
End

End				