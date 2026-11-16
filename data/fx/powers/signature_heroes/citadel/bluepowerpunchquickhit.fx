#########################################################
##	iceweapon
########################################################
FxInfo

LifeSpan	100

Input  
	Inpname	Chest
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	:BluePowerPunchHit.part
		Part2	:BluePowerPunchHit1.part
		Part3	:BluePowerPunchHit2.part
		Part4	:BluePowerPunchHitRaysSmall.part
		Sound PP10 100 100 .8
	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	:BluePowerPunchHit.part
		Part2	:BluePowerPunchHit1.part
		Part3	:BluePowerPunchHit2.part
		Part4	:BluePowerPunchHitRaysSmall.part
		Part2	:BluePowerPunchBallz.part
		Part4	:BluePowerPunchRingz.part
		Sound PP10 100 100 .8
	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End

End
