#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

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
		Ename	Hookup
		Type	local
		At	WepR
		geom	RiktiNode03

	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Flash
		Type	Start
		At	Hookup
		altpiv	5
		Part1	:BurstStreaksPistol.part
		Part2	:BurstEmbersPistol.part
		
	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	start 
		At	Hookup
		altpiv	5
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	:RiktiMachineGunBlueEnergyA.part
		Part2	:RiktiMachineGunDarkEnergy2.part
		Part3	:RiktiEnergyBullet3.part
		Part4	:RiktiEnergyBullet3.part
		
		Magnet	Mystic
		LookAt	Mystic
		
	End

	Event
		EName 	MussleFlash
		Type	local 
		At	Prime
		altpiv	1
		
		Part3	:RiktiMachineGunGlowStart2.part
		Part4	:RiktiMachineGunGlowStart2Opposite.part
		Sound RiktiPistol 100.0 100.0 .6
	End

	Event
		EName 	Prime2
		Type	start 
		At	Hookup
		altpiv	5
		Geom	LightDummy2
		bhvr	behaviors/spin3a.bhvr
		Magnet	Mystic
		LookAt	Mystic
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime2
		altpiv	1
		
		Part1	:RiktiEnergyStreamer3.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime2
		altpiv	2
		
		Part1	:RiktiEnergyStreamer3.part
				
	End


########################################################


	Event
		EName 	Prime3
		Type	start 
		At	Hookup
		altpiv	5
		Geom	LightDummy2
		bhvr	behaviors/spin3aOpposite.bhvr
		Magnet	Mystic
		LookAt	Mystic
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime3
		altpiv	1
		
		Part1	:RiktiEnergyStreamer3.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime3
		altpiv	2
		
		Part1	:RiktiEnergyStreamer3.part
				
	End

End


Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End

End


Condition
	On 	Prime2Hit
	
	Event
		Ename 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Prime3Hit
	
	Event
		Ename 	Prime3
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	200

	Event
		Ename 	all
		Type	Destroy
	End

End

End				