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
	InpName	WepR
End
##################################
Condition
	On 	Time
	Time 	25

	Event
		Ename	Hookup
		Type	local
		At	wepR
		geom	RiktiNode04
		Sound Riktigun_loop 60 60 .45

	End
	
	Event
		Ename	Flash
		Type	local
		At	Hookup
		altpiv	3
		Part1	WEAPONFX\BurstStreaks.part
		Part2	WEAPONFX\BurstEmbers.part
		
	End

End

Condition
	On 	Time
	Time 	28
	Event
		EName 	Prime
		Type	start 
		At	Hookup
		altpiv	3
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	WEAPONFX\RiktiEnergyBullet.part
		Part2	WEAPONFX\RiktiBlasterDarkEnergy.part
		Part3	WEAPONFX\RiktiBlasterBlueEnergy.part
		Part4	WEAPONFX\RiktiEnergyBullet.part
		Part5	WEAPONFX\RiktiEnergyBullet.part
		Magnet	Target
		LookAt	Target
		Sound RiktiAssault2 80.0 80.0 .6
		
	End

	Event
		EName 	MussleFlash
		Type	local 
		At	Prime
		altpiv	1
		Part1	WEAPONFX\BlasterGunGlowStart.part
		Part2	WEAPONFX\BlasterHead.part
		Part3	WEAPONFX\RiktiMachineGunGlowStart.part
		Part4	WEAPONFX\BlasterHeadLocal.part
	End

	Event
		EName 	Prime2
		Type	start 
		At	Hookup
		altpiv	3
		Geom	LightDummy2
		bhvr	behaviors/spin3.bhvr
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime2
		altpiv	1
		
		Part1	WEAPONFX\BlasterStreamer2.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime2
		altpiv	2
		
		Part1	WEAPONFX\BlasterStreamer2.part
				
	End


########################################################


	Event
		EName 	Prime3
		Type	start 
		At	Hookup
		altpiv	3
		Geom	LightDummy2
		bhvr	behaviors/spin3Opposite.bhvr
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime3
		altpiv	1
		
		Part1	WEAPONFX\BlasterStreamer2.part
				
	End

	Event
		EName 	s2
		Type	local 
		At	Prime3
		altpiv	2
		
		Part1	WEAPONFX\BlasterStreamer2.part
				
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