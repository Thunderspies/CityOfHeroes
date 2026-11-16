#########################################################
## Super Strength - Heavy Hit
########################################################

FxInfo

Lifespan 60

########################################################

#Condition
#	On 	Time
#	Time	3
#
#	Event
#		Ename	CameraShake
#		Type	Start
#		At	Root
#		ChildFX	:CameraShake_Minor.fx
#	End
#End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		bhvr	behaviors/ScaleBubble3.bhvr
		BhvrOverride
			RGB0 0 0 255
		End
		Geom	GEO_WhiteSphere
		Lifespan	4
	End

	Event
		EName 	Mallet
		Type	start
		At	Chest

		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		Sound ReichsmanPunch_01 100.0 100.0 .8
	End
End

########################################################

End