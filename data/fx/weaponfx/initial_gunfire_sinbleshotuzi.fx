#########################################################
##	
##

FxInfo
LifeSpan  120

##################################

Condition
	On 	Time
	Time 	41

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