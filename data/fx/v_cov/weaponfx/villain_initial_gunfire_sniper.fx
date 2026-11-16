#########################################################
##	
##

FxInfo
LifeSpan  155

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
	On	Time
	Time	83

	Event
		Type	Local
		At	WepR
		Sound titanprismblast 100 100 .5
	End
End


Condition
	On 	Time
	Time 	83


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
	Time 	79

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	Sniper01
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	75
	End

	Event
		EName 	Blast1
		Type	start
		At	p2
		Altpiv 1
		
		Sound laserrifleburst 100.0 100.0 1.0
	End

	Event
		Type	local
		At	p2
		Altpiv 1
		
		Part2	:Gunfire_Shotgunsmoke.part
		Part3	:Gunfire_ShotgunMuzzflashSmall.part
		lifespan	4
	End

	Event
		EName 	fire
		Type	local
		At	p2
		Altpiv 1
		Part1	:TorchflameAb.part
		Part2	:TorchflameBa.part
		Geom	FX_MachineGunBurst
		Bhvr	:VillianSniperlBlastColor.bhvr
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