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



		Part4	:BluePowerPunchHitRaysSmall.part
		sound HeavyHands_BoneSmasher_Hit 100 100 0.8
	End


	Event
		Type	start
		At	Chest
		Part	:Flash.part
		Part1	:BlueHitRing.part
		Part3	:BlueHitRing2.part
		Part	:BlueHitSmoke.part
		Part	:Sparks.part
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


End
