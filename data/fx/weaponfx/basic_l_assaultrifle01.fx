#########################################################
##	
##

FxInfo
LifeSpan  80

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepL
End

##################################
Condition
	On 	Time
	Time 	0

	Event
		ENAME   t2
		Type	Local 
		At	WepL
		#Part1	:Gunfire_AssaultShell2.Part
		Sound Assault3 120.0 120.0 1.0
	End

End

Condition
	On 	Time
	Time 	0

	Event
		ENAME   P2
		Type	Local 
		At	WepL
		
		Geom	FlameThrower
		
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

#Condition
#	On 	Time
#	Time	116
#
#	Event
#		Ename 	tracer
#		Type	Destroy
#	End
#
#End
#
#Condition
#	On 	Time
#	Time	111
#
#	Event
#		Ename 	T2
#		Type	Destroy
#	End
#End

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
	Time	21

	Event
		Ename 	p2
		Type	Destroy
	End
End


End				