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
		Geom	Sniper01
		#Part1	:Gunfire_AssaultShellBurst.Part
		Lifespan	4
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
		
		Sound sniper1 120.0 120.0 1.0
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		
		Part2	WEAPONFX/Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
	End

	Event
		EName 	fire
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflameAb.part
		Part2	WEAPONFX/TorchflameBa.part
		Geom	FX_MachineGunBurst
		#Part	:SniperFlash.part
		#Part	:SniperGlow.part
		lifespan	1
	End

End


##################################



End				