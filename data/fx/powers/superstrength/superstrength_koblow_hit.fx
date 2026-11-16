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
		ChildFX	:CameraShake_Major.fx
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
		Geom	SuperStrengthBubble
		bhvr	behaviors/ScaleBubble.bhvr
	End

	Event
		EName 	Mallet2
		Type	start
		At	Chest

		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Part5	:SuperStrengthHitGlows.part
		Sound PunchHit2 100.0 100.0 .8
	End

########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:MindGlowHit2.part
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