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
	Time 	0

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	Sniper02
		#Part1	:Gunfire_AssaultShellBurst.Part
		Lifespan	4
	End

	Event
		EName 	Blast1
		Type	start
		At	p2
		Altpiv 1
		#Geom	FX_MachineGunBurst
		#Part1	:SniperFlash.part
		#Part2	:SniperGlow.part
		Sound tranqdart 120.0 120.0 1.0
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

	Event
		ENAME   shell
		Type	Local 
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
	End

End


##################################


End				