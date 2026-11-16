#########################################################
## Rikti Sword-Gun - Ranged Attack	
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	SwordGun_Offset
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0 0 4
		End
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Ename	Flash
		Type	Local
		At	SwordGun_Offset
		Part1	WEAPONFX\BurstStreaks.part
		Part2	WEAPONFX\BurstEmbers.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Start 
		At	SwordGun_Offset
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part1	WEAPONFX\RiktiBlasterBullet.part
		Part2	WEAPONFX\RiktiMachineGunDarkEnergy.part
		Part3	WEAPONFX\RiktiMachineGunBlueEnergy.part
		Part4	WEAPONFX\RiktiBlasterBullet.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	MussleFlash
		Type	Local 
		At	Prime
		altpiv	1
		Part1	WEAPONFX\RiktiMachineGunGlowStart1.part
		Part2	WEAPONFX\RiktiMachineGunGlowStart.part
		Sound RiktiAssault2 100.0 100.0 .6
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime2
		Type	Start 
		At	SwordGun_Offset
		Geom	LightDummy2
		bhvr	behaviors/spin3b.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	s1
		Type	Local 
		At	Prime2
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End

	Event
		EName 	s2
		Type	Local 
		At	Prime2
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime3
		Type	Start 
		At	SwordGun_Offset
		Geom	LightDummy2
		bhvr	behaviors/spin3bOpposite.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	s1
		Type	Local 
		At	Prime3
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End

	Event
		EName 	s2
		Type	Local 
		At	Prime3
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End
End

#########################################################

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

#########################################################

End