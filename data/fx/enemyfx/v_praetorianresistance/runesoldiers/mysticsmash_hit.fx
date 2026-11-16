#############################################################
## PowerPunch_Hit.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part4	:BluePowerPunchHitRaysSmall2.part
		sound HeavyHands_PowerPunch_Hit 100 100 0.8
	End


	Event
		Type	start
		At	Chest
		Part1	:BlueHitRing_Small.part
		Part	:Sparks.part
		Part3	:BlueHitRing2_Small.part

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		300
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

#############################################################

End
