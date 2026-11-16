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
		EName 	Prime
		Type	Start
		At	p2
		Altpiv 2
		
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
		Geom	Mastermind_Military_Rifle
	End

	Event
		Type	Local 
		At	WepR
#		Part1	:Gunfire_AssaultShellBurst.Part
	End

	Event
		EName 	Blast1
		Type	start
		At	p2
		Altpiv 2
		
		Sound SCARSilencer 100.0 100.0 1.0
		Sound SCARSilencer2 100.0 100.0 1.0
		Sound SCARSilencer3 100.0 100.0 1.0
	End

	Event
		Type	local
		At	p2
		Altpiv 2
		
#		BHVR	WEAPONs\WeaponRotate.bhvr
		Part2	WEAPONFX/SpecOpps_Shotgunsmoke.part
		Part3	WEAPONFX/SpecOpps_ShotgunMuzzflashSmall.part
	End

	Event
		EName 	fire
		Type	local
		At	p2
		Altpiv 2
		
		Part1	WEAPONFX/TorchflameBlue1.part
		Part2	WEAPONFX/TorchflameBlue2.part

		lifespan	1
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		#Geom	FX_MachineGunBurst
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
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

Condition
	On 	Time
	Time	49

	Event
		Ename 	all
		Type	Destroy
	
	End

End				


End				