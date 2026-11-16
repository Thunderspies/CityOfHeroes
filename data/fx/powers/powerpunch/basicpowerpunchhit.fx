#############################################################
## BasicPowerPunchHit.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Input
	Inpname	Chest
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Chest
		Part1	POWERS\PowerPunch\PowerPunchHit.part
		Part2	POWERS\PowerPunch\PowerPunchHit1.part
		Part3	POWERS\PowerPunch\PowerPunchHit2.part
		Part4	POWERS\PowerPunch\PowerPunchHitRaysSmall.part
		Sound PP10 100 100 0.8
		Sound PP10a 100 100 0.8
		Sound PP10b 100 100 0.8
	End

	Event
		Type	start
		At	Chest
		Part2	POWERS\PowerPunch\PowerPunchBallz.part
		Part4	POWERS\PowerPunch\PowerPunchRingz.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End