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
	Time 	0

	Event
		ENAME   t2
		Type	Local 
		At	WepR
		
		Part1	:Gunfire_AssaultShell2.Part
		Sound machgun3 120.0 120.0 .9
	End

End

Condition
	On 	Time
	Time 	0

	Event
		ENAME   P2
		Type	Local 
		At	WepR
		
		Geom	FlameThrower
		Lifespan	138
		
	End

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
	Time	116

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	111

	Event
		Ename 	T2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	3

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
	Time 	6

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

	Event
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

End

Condition
	On 	Time
	Time 	9

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
	Time 	12

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
	Time 	15

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
	Time 	16

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
	Time 	21

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
	Time 	24

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
	Time 	27

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
	Time 	30

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
	Time 	33

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
	Time 	36

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
	Time 	39

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
	Time 	42

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
	Time 	45

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
	Time 	48

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
	Time 	51

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
	Time 	54

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
	Time 	57

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
	Time 	60

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
	Time 	63

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
	Time 	66

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
	Time 	69

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
	Time 	72

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
	Time 	75

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
	Time 	78

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
	Time 	81

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
	Time 	84

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
	Time 	87

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
	Time 	90

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
	Time 	93

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
	Time 	96

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
	Time 	99

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
	Time 	102

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
	Time 	105

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
	Time 	108

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
	Time 	111

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
	Time 	114

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
	Time 	117

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
	Time 	120

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
	Time 	123

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
	Time 	126

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
	Time	3

	Event
		Ename 	Blast1
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	6

	Event
		Ename 	Blast2
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	9

	Event
		Ename 	Blast3
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	12

	Event
		Ename 	Blast4
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	15

	Event
		Ename 	Blast5
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	18

	Event
		Ename 	Blast6
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	21

	Event
		Ename 	Blast5
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	24

	Event
		Ename 	Blast6
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	27

	Event
		Ename 	Blast7
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	30

	Event
		Ename 	Blast8
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	33

	Event
		Ename 	Blast9
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	36

	Event
		Ename 	Blast10
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	39

	Event
		Ename 	Blast11
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	42

	Event
		Ename 	Blast12
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	45

	Event
		Ename 	Blast13
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	48

	Event
		Ename 	Blast14
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	51

	Event
		Ename 	Blast15
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	54

	Event
		Ename 	Blast16
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	57

	Event
		Ename 	Blast17
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	60

	Event
		Ename 	Blast18
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	63

	Event
		Ename 	Blast19
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	66

	Event
		Ename 	Blast20
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	69

	Event
		Ename 	Blast21
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	72

	Event
		Ename 	Blast22
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	75

	Event
		Ename 	Blast23
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	78

	Event
		Ename 	Blast24
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	81

	Event
		Ename 	Blast25
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	84

	Event
		Ename 	Blast26
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	87

	Event
		Ename 	Blast27
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	90

	Event
		Ename 	Blast28
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	93

	Event
		Ename 	Blast29
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	96

	Event
		Ename 	Blast30
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	99

	Event
		Ename 	Blast31
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	102

	Event
		Ename 	Blast32
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	105

	Event
		Ename 	Blast33
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	108

	Event
		Ename 	Blast34
		Type	Destroy
	End
End



Condition
	On 	Time
	Time	111

	Event
		Ename 	Blast35
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	114

	Event
		Ename 	Blast36
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	117

	Event
		Ename 	Blast37
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	120

	Event
		Ename 	Blast38
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	123

	Event
		Ename 	Blast39
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	126

	Event
		Ename 	Blast40
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	129

	Event
		Ename 	Blast41
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	132

	Event
		Ename 	Blast42
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	135

	Event
		Ename 	Blast43
		Type	Destroy
	End
End


End				