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
	Time 	49


	Event
		ENAME   p1
		Type	Local 
		At	WepR
		Geom	Sniper01
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		Part	:VillianSniper_Tracer.part
		Bhvr	:VillianSniperlTracer.bhvr
		Magnet	Target
		Lookat	Target

	End
	
End

Condition
	On 	Time
	Time 	45

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
		
		Sound advancedassault 100.0 100.0 1.0
	End

	Event
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


Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End


End				