#########################################################
## Super Strength - KO Blow Hit
########################################################

FxInfo

LifeSpan 60

#########################################################

Input
	Inpname	Hips
End

#########################################################

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
		EName 	Mallet
		Type	start
		At	Chest
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_SuperStrengthBubble
		bhvr	CustomizeablePowers\SuperStrength\ScaleBubble.bhvr
	End

	Event
		EName 	Mallet2
		Type	start
		At	Chest

		Part1	CustomizeablePowers\SuperStrength\SuperStrengthHitEmber.part
		Part2	CustomizeablePowers\SuperStrength\SuperStrengthHitStar.part
		Part3	CustomizeablePowers\SuperStrength\SuperStrengthHitRing.part
		Part4	CustomizeablePowers\SuperStrength\SuperStrengthHitRing2.part
		Part5	CustomizeablePowers\SuperStrength\SuperStrengthHitGlows.part
		Sound PunchHit2 100.0 100.0 .8
	End

########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	CustomizeablePowers\SuperStrength\HeadDizzies.part
		Part2	CustomizeablePowers\SuperStrength\HeadDizzyRings.part
		part3	CustomizeablePowers\SuperStrength\MindGlowHit2.part
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