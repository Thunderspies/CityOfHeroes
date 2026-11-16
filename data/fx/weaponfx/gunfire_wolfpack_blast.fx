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
	Time 	60

	Event
		Ename	Flash
		Type	local
		At	Mystic
		Part1	:BurstStreaks.part
		Part2	:BurstEmbers.part
		
	End

End

Condition
	On 	Time
	Time 	62

	Event
		EName 	Prime
		Type	start 
		At	Mystic
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	:RiktiBlasterBullet.part
		Part2	:RiktiMachineGunDarkEnergy.part
		Part3	:RiktiMachineGunBlueEnergy.part
		Part4	:RiktiBlasterBullet.part
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	MussleFlash
		Type	local 
		At	Prime
		altpiv	1
		
		Part1	:RiktiMachineGunGlowStart1.part
		Part2	:RiktiMachineGunGlowStart.part
		Sound RiktiAssault2 100.0 100.0 .6
		
	End

	Event
		EName 	Prime2
		Type	start 
		At	Mystic
		Geom	LightDummy2
		bhvr	behaviors/spin3b.bhvr
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime2
		altpiv	1
		
		Part1	:RiktiEnergyStreamer2.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime2
		altpiv	2
		
		Part1	:RiktiEnergyStreamer2.part
				
	End


########################################################


	Event
		EName 	Prime3
		Type	start 
		At	Mystic
		Geom	LightDummy2
		bhvr	behaviors/spin3bOpposite.bhvr
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime3
		altpiv	1
		
		Part1	:RiktiEnergyStreamer2.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime3
		altpiv	2
		
		Part1	:RiktiEnergyStreamer2.part
				
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
	Time	47

	Event
		Ename 	shells
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