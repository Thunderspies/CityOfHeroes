#########################################################
##	
##

FxInfo

LifeSpan  100

##################################

Condition
	On 	Time
	Time 	31

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	TommyGunWepR
		#Part1	:Gunfire_AssaultShell.Part
		Sound Assault2 120.0 120.0 1.0

		LifeSpan  5
	
	End

	Event
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

	Event
		EName 	Blast1
		Type	start
		At	p2
		Altpiv 1
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		
		LifeSpan  3
	
	End

	Event
		EName 	tracer
		Type	start
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameAb.part
		Part2	WEAPONFX/TorchflameBa.part
		
		LifeSpan  2
	
	End

End

End

#Condition
#	On 	Time
#	Time 	32
#
#	Event
#		EName 	Blast2
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	35
#
#	Event
#		EName 	Blast3
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	38
#
#	Event
#		EName 	Blast4
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	41
#
#	Event
#		EName 	Blast5
#		Type	local
#		At	p2
#		Altpiv 1
#		Geom	FX_MachineGunBurst
#		Part1	:MachineGunFlash.part
#		Part2	:MachineGunGlow.part
#	End
#
#End
#
#
#
#
###################################
#
#Condition
#	On 	Time
#	Time	34
#
#	Event
#		Ename 	Blast1
#		Type	Destroy
#	End
#End
#
#
#Condition
#	On 	Time
#	Time	37
#
#	Event
#		Ename 	Blast2
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	40
#
#	Event
#		Ename 	Blast3
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	43
#
#	Event
#		Ename 	Blast4
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Time
#	Time	45
#
#	Event
#		Ename 	All
#		Type	Destroy
#	End
#End
#
#End				