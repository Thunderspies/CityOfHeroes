#############################################################
## FireBall.fx
#############################################################

FxInfo

LifeSpan 200

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
	Event
		EName	InitialSwordFire
		Type	Local
		At	OffsetAnchor
		Part	:FireHands.part
		Sound Fireball1 100.0 100.0 .8
		LifeSpan 45
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
		LifeSpan 14
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
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		120
			PhysForcePowerJitter	25
		End
		Part	:FireBallHead.part
		Part	:FireBallTail.part
		Part	:FireBallStreak.part
		Part	:SparkTrail.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

#############################################################

End