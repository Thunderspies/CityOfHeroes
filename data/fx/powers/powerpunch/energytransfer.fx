#############################################################
## EnergyTransfer.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	hand1
		Type	Local
		At	WepL
		Part5	:Largepowerpunch3.part
		Part1	:BuildupRings.part
		Part2	:BuildupSparks.part
		Part3   :BuildupGlow.part
		Sound chargeUp_1 50 50 .8
		Lifespan 30
	End

	Event
		EName 	hand2
		Type	Local
		At	WepR
		Part5	:Largepowerpunch3.part
		Part1	:BuildupRings.part
		Part2	:BuildupSparks.part
		Part3   :BuildupGlow.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	63

	Event
		EName 	handA
		Type	Local
		At	WepR
		Part3   POWERS\PowerPunch\QuickRings.part
	End

	Event
		EName 	handB
		Type	Local
		At	WepL

		Part3   POWERS\PowerPunch\QuickRings.part
	End
End

## SELF HIT ###########################################################

#Condition
#	On 	Time
#	Time 	55
#
#	Event
#		Type	start
#		At	Chest
#		Part1	POWERS\PowerPunch\PowerPunchHit.part
#		Part2	POWERS\PowerPunch\PowerPunchHit1.part
#		Part3	POWERS\PowerPunch\PowerPunchHit2.part
#		Part4	POWERS\PowerPunch\PowerPunchHitRaysSmall.part
#		Sound	PP12 100 30 0.75
#	End
#
#	Event
#		Type	start
#		At	Chest
#		Part1	POWERS\PowerPunch\HitRing.part
#		Part2	POWERS\PowerPunch\PowerPunchBallz.part
#		Part3	POWERS\PowerPunch\HitRing2.part
#		Part4	POWERS\PowerPunch\PowerPunchRingz.part
#	End
#
#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	Head
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		20
#			PhysForcePower		350
#			PhysForcePowerJitter	50
#		End
#		Lifespan 1
#	End
#End

## AOE HIT ###########################################################

#Condition
#	On 	Time
#	Time 	64
#
#	Event
#		EName 	Impact
#		Type	Posit
#		At	Chest
#		Lookat	Target
#	End
#
#	Event
#		EName 	Explode
#		Type	Local
#		At	Impact
#		Bhvr	behaviors\CameraShake05Subtle.bhvr
#		BhvrOverride
#			PositionOffset	0 0 5
#		End
#		Part   V_COV\EnemyPowers\SignatureHeroes\BackAlleyBrawler\ExplosiveFlash.part
#		Part   V_COV\EnemyPowers\SignatureHeroes\BackAlleyBrawler\ExplosiveRings.part
#		Part   V_COV\EnemyPowers\SignatureHeroes\BackAlleyBrawler\ExplosiveRays.part
#		LifeSpan 5
#	End
#
#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	Explode
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		25
#			PhysForcePower		1500
#			PhysForcePowerJitter	200
#		End
#		Lifespan 1
#	End
#End

#############################################################

End