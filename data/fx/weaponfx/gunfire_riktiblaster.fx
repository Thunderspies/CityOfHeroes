#############################################################
## Gunfire_RiktiBlaster.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	6

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
		Part1	:BurstStreaks.part
		Part2	:BurstEmbers.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	Hookup
		altpiv	3
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	:RiktiEnergyBullet.part
		Part2	:RiktiBlasterDarkEnergy.part
		Part3	:RiktiBlasterBlueEnergy.part
		Part4	:RiktiEnergyBullet.part
		Part5	:RiktiEnergyBullet.part
		Magnet	Target
		LookAt	Target
		Sound RiktiAssault2 80.0 80.0 .6
	End

	Event
		EName 	MussleFlash
		Type	local
		At	Prime
		altpiv	1
		Part1	:BlasterGunGlowStart.part
		Part2	:BlasterHead.part
		Part3	:RiktiMachineGunGlowStart.part
		Part4	:BlasterHeadLocal.part
	End
End

Condition
	On 	Time
	Time 	7

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
		Part1	:BlasterStreamer2.part
	End

	Event
		EName 	s2
		Type	local
		At	Prime2
		altpiv	2
		Part1	:BlasterStreamer2.part
	End
End

Condition
	On 	Time
	Time 	7

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
		Part1	:BlasterStreamer2.part
	End

	Event
		EName 	s2
		Type	local
		At	Prime3
		altpiv	2
		Part1	:BlasterStreamer2.part
	End
End

#############################################################

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

#############################################################

End