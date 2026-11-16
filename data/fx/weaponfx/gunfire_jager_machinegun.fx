#########################################################
##	
##

FxInfo
LifeSpan  75

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End 

Flags    InheritAlpha


##################################
Condition
	On 	Time
	Time 	13

	Event
		Ename	p2
		Type	local
		At	WepR
		Bhvr	Behaviors/Jager_Machine_Fade_UP.bhvr
		Geom	JagerMachineGun
		Sound JaegerMachGun5 90 90 1.0
		LifeSpan	65
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		
		Part	:nemesisMuzzflashSmoke2.part
		lifespan	68
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 3
		
		Part	:nemesisMuzzflashSmoke2.part
		lifespan	68
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	WepR
		
		Part1	:Gunfire_AssaultShell3.Part
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA2.part
		Part2	WEAPONFX/TorchflameB2.part
		
		lifespan	40
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 3
		Part1	WEAPONFX/TorchflameA2.part
		Part2	WEAPONFX/TorchflameB2.part
		
		lifespan	40
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End


Condition
	On 	Time
	Time 	35

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	38

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	41

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	43

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	46

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	49

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	52

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	58

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	61

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	64

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	67

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End

Condition
	On 	Time
	Time 	70

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 3
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		
		Lifespan	3
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 4
		
		Part1	:GattellinGunFlash2.part
		Part2	:GattellinGunGlow2.part
		Lifespan	3
	End

End



##################################

Condition
	On 	Time
	Time	75

	Event
		Ename 	All
		Type	Destroy
	End
End

End				