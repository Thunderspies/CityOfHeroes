#############################################################
## FireBallTarget.fx
#############################################################

FxInfo
LifeSpan 100

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  9

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		hardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\Subtractive\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	:FireWeaponExplosion.part
		Part2	:FireWeaponHitSpark.part
		Lifespan 6
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End
End

#############################################################

End
