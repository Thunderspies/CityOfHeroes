#########################################################
##	
##

FxInfo
LifeSpan  220

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
	Time 	35

	Event
		ENAME   t2
		Type	Local 
		At	WepR
		
		#Part1	:Gunfire_AssaultShell2.Part
		Sound Assault_long2 120.0 120.0 .9
	End

End

Condition
	On 	Time
	Time 	34

	Event
		ENAME   P2
		Type	Local 
		At	WepR	
		Geom	FlameThrower
		Lifespan	135
		
	End

#	Event
#		EName 	BarrelSpin
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_ChainGunBarrell
#		Bhvr	Behaviors/BarrellSpin3.bhvr
#		lifespan	169
#	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time	150

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	155

	Event
		Ename 	T2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	37

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	40

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	43

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	46

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	49

	Event
		EName 	Blast6
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	52

	Event
		EName 	Blast7
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	55

	Event
		EName 	Blast8
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	58

	Event
		EName 	Blast9
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	61

	Event
		EName 	Blast10
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	64

	Event
		EName 	Blast11
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	67

	Event
		EName 	Blast12
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	70

	Event
		EName 	Blast13
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	73

	Event
		EName 	Blast14
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	76

	Event
		EName 	Blast15
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	79

	Event
		EName 	Blast16
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	82

	Event
		EName 	Blast17
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	85

	Event
		EName 	Blast18
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	88

	Event
		EName 	Blast19
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	91

	Event
		EName 	Blast20
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	94

	Event
		EName 	Blast21
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	97

	Event
		EName 	Blast22
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	100

	Event
		EName 	Blast23
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	103

	Event
		EName 	Blast24
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	106

	Event
		EName 	Blast25
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	109

	Event
		EName 	Blast26
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	112

	Event
		EName 	Blast27
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	115

	Event
		EName 	Blast28
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	118

	Event
		EName 	Blast29
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	121

	Event
		EName 	Blast30
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	124

	Event
		EName 	Blast31
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	127

	Event
		EName 	Blast32
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	130

	Event
		EName 	Blast33
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	133

	Event
		EName 	Blast34
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	136

	Event
		EName 	Blast35
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	139

	Event
		EName 	Blast36
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	142

	Event
		EName 	Blast37
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	145

	Event
		EName 	Blast38
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	148

	Event
		EName 	Blast39
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	151

	Event
		EName 	Blast40
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	154

	Event
		EName 	Blast41
		Type	local
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
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
	Time 	157

	Event
		EName 	Blast42
		Type	local
		At	p2
		Altpiv 1
		
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
	Time 	160

	Event
		EName 	Blast43
		Type	local
		At	p2
		Altpiv 1
		
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

##################################################################

Condition
	On 	Time
	Time	37

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	40

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	46

	Event
		Ename 	Blast4
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	49

	Event
		Ename 	Blast5
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	52

	Event
		Ename 	Blast6
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	55

	Event
		Ename 	Blast5
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	58

	Event
		Ename 	Blast6
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	61

	Event
		Ename 	Blast7
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	64

	Event
		Ename 	Blast8
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	67

	Event
		Ename 	Blast9
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	70

	Event
		Ename 	Blast10
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	73

	Event
		Ename 	Blast11
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	76

	Event
		Ename 	Blast12
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	79

	Event
		Ename 	Blast13
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	82

	Event
		Ename 	Blast14
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	85

	Event
		Ename 	Blast15
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	88

	Event
		Ename 	Blast16
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	91

	Event
		Ename 	Blast17
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	94

	Event
		Ename 	Blast18
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	97

	Event
		Ename 	Blast19
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	100

	Event
		Ename 	Blast20
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	103

	Event
		Ename 	Blast21
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	106

	Event
		Ename 	Blast22
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	109

	Event
		Ename 	Blast23
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	112

	Event
		Ename 	Blast24
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	115

	Event
		Ename 	Blast25
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	118

	Event
		Ename 	Blast26
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	121

	Event
		Ename 	Blast27
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	124

	Event
		Ename 	Blast28
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	127

	Event
		Ename 	Blast29
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	130

	Event
		Ename 	Blast30
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	133

	Event
		Ename 	Blast31
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	136

	Event
		Ename 	Blast32
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	139

	Event
		Ename 	Blast33
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	142

	Event
		Ename 	Blast34
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	145

	Event
		Ename 	Blast35
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	148

	Event
		Ename 	Blast36
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	151

	Event
		Ename 	Blast37
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	154

	Event
		Ename 	Blast38
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	157

	Event
		Ename 	Blast39
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	160

	Event
		Ename 	Blast40
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	163

	Event
		Ename 	Blast41
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	166

	Event
		Ename 	Blast42
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	169

	Event
		Ename 	Blast43
		Type	Destroy
	End
End


End				