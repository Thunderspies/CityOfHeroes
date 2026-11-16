#########################################################
##	
##

FxInfo
LifeSpan  350

Flags    InheritAlpha

##################################


Condition
	On 	cycle
	Time 	60
	chance	.95

	Event
		Type	local
		At	WepR
		BHVR	:BallTurentLightPosition.bhvr
		part	:BallTurrentBlinkingLight.part
		part	:BallTurrentLazer.part

		LifeSpan  92

	End


End

Condition
	On 	Time
	Time 	60

	
	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		ENAME   sound
		Type	Local 
		At	WepR
		Part1	:Gunfire_AssaultShell.Part
		Sound Assault3 120.0 120.0 1.0
	End

	Event
		EName 	Blast1
		Type	local
		At	WepR
		BHVR	:TurentBlastSpin.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	WepR
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		
	End

End

Condition
	On 	Time
	Time 	61

	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		EName 	Blast2
		Type	local
		At	WepR
		BHVR	:TurentBlastSpin.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End


Condition
	On 	Time
	Time 	64

	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		EName 	Blast3
		Type	local
		At	WepR
		BHVR	:TurentBlastSpin.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	67

	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		EName 	Blast4
		Type	local
		At	WepR
		BHVR	:TurentBlastSpin.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End

Condition
	On 	Time
	Time 	70

	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		EName 	Blast5
		Type	local
		At	WepR
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

End


Condition
	On 	Time
	Time 	73

	Event
		Type	Start
		At	WepR
		
		Part	:BallTurret50Cal_fire.part
		
		Magnet	Chest

	End

	Event
		EName 	Blast5
		Type	local
		At	WepR
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		LifeSpan	1
	End

End


##################################

Condition
	On 	Time
	Time	63

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	66

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	69

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	72

	Event
		Ename 	Blast4
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	74

	Event
		Ename 	sound
		Type	Destroy
	End
	
	Event
		Ename 	tracer
		Type	Destroy
	End

	Event
		Ename 	Blast5
		Type	Destroy
	End
End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End

End				