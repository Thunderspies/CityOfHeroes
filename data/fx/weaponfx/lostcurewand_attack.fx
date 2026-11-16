#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Offset
		Type	Local
		At	WepR
		Geom	Staff_02
	End

	Event
		Ename	Hookup
		Type	Local
		At	Offset
		AltPiv	1
	End
End

## ATTACK ###########################################################

Condition
	On 	Time
	Time 	26

	Event
		Type	Local
		At	Hookup
		Part1	WEAPONFX\BurstStreaksPistol.part
		Part2	WEAPONFX\BurstEmbersPistol.part
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime
		Type	start
		At	Hookup
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	WEAPONFX\RiktiMachineGunBlueEnergyA.part
		Part2	WEAPONFX\RiktiMachineGunDarkEnergy2.part
		Part3	WEAPONFX\RiktiEnergyBullet3.part
		Part4	WEAPONFX\RiktiEnergyBullet3.part
		Geom	CenterDummy
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	MussleFlash
		Type	local
		At	Prime
		altpiv	1
		Part3	WEAPONFX\RiktiMachineGunGlowStart2.part
		Part4	WEAPONFX\RiktiMachineGunGlowStart2Opposite.part
		Sound Demutate_01 100.0 100.0 .6
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime2
		Type	start
		At	Hookup
		bhvr	behaviors\spin3a.bhvr
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	Prime2
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer3.part
	End

	Event
		Type	local
		At	Prime2
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer3.part
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime3
		Type	start
		At	Hookup
		bhvr	behaviors\spin3aOpposite.bhvr
		Geom	LightDummy2
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	Prime3
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer3.part
	End

	Event
		Type	local
		At	Prime3
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer3.part
	End
End

## ON HIT ###########################################################

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

#############################################################

End