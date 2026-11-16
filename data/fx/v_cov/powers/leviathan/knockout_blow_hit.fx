########################################################
## Super Strength - KO Blow Hit
########################################################

FxInfo

LifeSpan 60

########################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	CameraShake
		Type	Start
		At	Root
		ChildFX	POWERS\SuperStrength\CameraShake_Major.fx
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Target

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
	End

########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		part 	:Flash.part
		part	:StrengthBodyGlows.part
		Part	:HeadDizzies.part
		Part	:HeadDizzyRings.part
		part	:ERing01.part
		Part	POWERS\MartialArts\MartialArtsHitRaysLarge2.part
		Part	POWERS\MartialArts\MAHitGlow01.part
		#Part4	POWERS\MartialArts\MAHitStar02Lrg.part
	End
End

########################################################

Condition
	On 	Time
	Time 	6

	Event
		EName 	Mallet
		Type	Destroy
	End

	Event
		EName 	Mallet2
		Type	Destroy

	End
End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Core
		Type	Destroy
	End
End

########################################################

End