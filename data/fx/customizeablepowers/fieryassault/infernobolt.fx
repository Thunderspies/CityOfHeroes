#############################################################
## InfernoBolt.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FieryAssault\FireHandz.part
		#Sound	Flamebolt4 100.0 30.0 .9

	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FieryAssault\FireHandz.part


	End

End

Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash1
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FieryAssault\FireHandsFlash.part
		Sound Flamebolt4 100.0 100.0 .9

	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FieryAssault\FireHandsFlash.part

	End

End


Condition
	On 	Time
	Time 	24

	Event
		EName	SwordFlash1
		Type	Destroy


	End

	Event
		EName	SwordFlash2
		Type	Destroy


	End
End

Condition
	On 	Time
	Time 	45

	Event
		Ename	Poo
		Type	Destroy
	End

	Event
		EName	InitialSwordFire
		Type	Destroy


	End

End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime
		Type	Start
		At	BendMystic
		Bhvr	CustomizeablePowers\FieryAssault\Fastprojectile4.bhvr

		Magnet	Target
		LookAt	Target

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		125
			PhysForcePowerJitter	25
		End
	End

	Event
		EName 	Prime2
		Type	Start
		At	BendMystic
		Bhvr	CustomizeablePowers\FieryAssault\InfernoBoltBeam.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FireBlast1
		Magnet	Target
		LookAt	Target

		LifeSpan		20

	End

	Event
		EName 	Prime3
		Type	Start
		At	BendMystic
		Bhvr	CustomizeablePowers\FieryAssault\InfernoBoltBeamSmall.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FireBlast2
		Magnet	Target
		LookAt	Target

		LifeSpan		20

	End

	Event
		EName	poo
		Type	local
		At	prime2
		altpiv	1
		Bhvr	CustomizeablePowers\FieryAssault\EnergyBlastHeadInner.bhvr
				BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FireBlastCone1
		Part1	CustomizeablePowers\FieryAssault\WhiteFlame.part

		LifeSpan		5

	End

	Event
		EName	poo2
		Type	local
		At	prime2
		altpiv	1
		Bhvr	CustomizeablePowers\FieryAssault\EnergyBlastHead.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FireBlastCone1

		LifeSpan		5

	End

	Event
		EName	prime4
		Type	start
		At	BendMystic
		Bhvr	CustomizeablePowers\FieryAssault\MedianprojectileFire.bhvr
			#Part1	:BeamFire3.part
			#Part2	:BeamFire.part
		Part3	CustomizeablePowers\FieryAssault\BeamFire2.part


		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	Time
	Time 	24

	Event
		EName	prime5
		Type	start
		At	BendMystic
		Bhvr	CustomizeablePowers\FieryAssault\MedianprojectileFire.bhvr
		Part1	CustomizeablePowers\FieryAssault\BeamFire.part
		Part2	CustomizeablePowers\FieryAssault\BeamFire2.part
		Part3	CustomizeablePowers\FieryAssault\BeamFire3.part
		#Part4	:BeamSmoke.part

		Magnet	Target
		LookAt	Target
	End


End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Poo2
		Type	Destroy
	End

End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End

End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End

End

Condition
	On	Prime4Hit

	Event
		Ename	Prime4
		Type	Destroy
	End

End

Condition
	On	Prime5Hit

	Event
		Ename	Prime5
		Type	Destroy
	End

End

End


