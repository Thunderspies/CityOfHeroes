#############################################################
## Gunfire_RiktiAssault.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	WepR
		geom	RiktiNode02
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Ename	Flash
		Type	local
		At	Hookup
		altpiv	5
		Part1	:BurstStreaks.part
		Part2	:BurstEmbers.part
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
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	Hookup
		altpiv	5
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	:RiktiBlasterBullet.part
		Part2	:RiktiMachineGunDarkEnergy.part
		Part3	:RiktiMachineGunBlueEnergy.part
		Part4	:RiktiBlasterBullet.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime2
		Type	start
		At	Hookup
		altpiv	5
		bhvr	behaviors\spin3b.bhvr
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2Streamer1
		Type	local
		At	Prime2
		altpiv	1
		Part1	:RiktiEnergyStreamer2.part
	End

	Event
		EName 	Prime2Streamer2
		Type	local
		At	Prime2
		altpiv	2
		Part1	:RiktiEnergyStreamer2.part
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime3
		Type	start
		At	Hookup
		altpiv	5
		bhvr	behaviors\spin3bOpposite.bhvr
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime3Streamer1
		Type	local
		At	Prime3
		altpiv	1
		Part1	:RiktiEnergyStreamer2.part
	End

	Event
		EName 	Prime3Streamer2
		Type	local
		At	Prime3
		altpiv	2
		Part1	:RiktiEnergyStreamer2.part
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

	Event
		Ename 	Prime2Streamer1
		Type	Destroy
	End

	Event
		Ename 	Prime2Streamer2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		Ename 	Prime3
		Type	Destroy
	End

	Event
		Ename 	Prime3Streamer1
		Type	Destroy
	End

	Event
		Ename 	Prime3Streamer2
		Type	Destroy
	End
End

#############################################################

End