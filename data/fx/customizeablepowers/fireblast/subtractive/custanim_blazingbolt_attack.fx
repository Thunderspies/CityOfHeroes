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
		Part1	:FireHandz.part
		Lifespan 45
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	SwordFlash1
		Type	Local
		At	WepR
		Part1	:FireHandsFlash.part
		Sound Flamebolt4 100.0 100.0 .9
		Lifespan 14
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\Fastprojectile4.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		125
			PhysForcePowerJitter	25
		End
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	:InfernoBoltBeam.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Dark_FireBlast1
		Magnet	Target
		LookAt	Target
		LifeSpan 20
	End

	Event
		EName	poo
		Type	local
		At	prime2
		altpiv	1
		Bhvr	behaviors\EnergyBlastHeadInner.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Dark_FireBlastCone1
		Part1	:WhiteFlame.part
		LifeSpan 5
	End

	Event
		EName	poo2
		Type	local
		At	prime2
		altpiv	1
		Bhvr	behaviors\EnergyBlastHead.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Dark_FireBlastCone1
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime3
		Type	Start
		At	WepR
		Bhvr	behaviors\InfernoBoltBeamSmall.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FireBlast2
		Magnet	Target
		LookAt	Target
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName	prime4
		Type	start
		At	WepR
		Bhvr	Behaviors\MedianprojectileFire.bhvr
		Part3	:BeamFire2.part
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
		At	WepR
		Bhvr	Behaviors\MedianprojectileFire.bhvr
		Part1	:BeamFire.part
		Part2	:BeamFire2.part
		Part3	:BeamFire3.part
		Magnet	Target
		LookAt	Target
	End
End

############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
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

#############################################################

End