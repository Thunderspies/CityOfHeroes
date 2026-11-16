#############################################################
## FireBlast_Attack.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	OffsetAnchor
		Type	Local
		Geom	FX_WWMobileDefenseTurret_Offsets
	End
End

Condition
	On 	Time
	Time  	8

	Event
		Type	Local
		At	OffsetAnchor
		Sound Fireball1 60.0 60.0 .33
	End
End

Condition
	On	Time
	Time	16

	Event
		Type 	Local
		At 	OffsetAnchor
		Sound Fireblastfly 80 80 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	OffsetAnchor
		Part1	:FireHandz.part
		#Sound	Fireball1 100.0 30.0 .66
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	SwordFlash2
		Type	Local
		At	OffsetAnchor
		Part1	:FireHandsFlash.part
		Lifespan 14
	End
End

#############################################################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start
		At	OffsetAnchor
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		80
			PhysForcePowerJitter	15
		End
		LookAt	Target
		Part1	:FireHands2.part
		Part5	:FireHands2.part
		Part2	:FireBoltTail2.part
		Part4	:SparkTrail.part
		Magnet	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

#############################################################

End