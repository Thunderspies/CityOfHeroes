#########################################################
## Super Strength - Hurl Hit
########################################################

FxInfo

Lifespan 60

########################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	CameraShake
		Type	Start
		At	Root
		ChildFX	:CameraShake_Medium.fx
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
		BhvrOverride
			FadeOutLength		20
		End
		bhvr	behaviors/ScaleBubble.bhvr
		Lifespan 5	
	End

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Part1	:SuperStrengthHitEmber.part
		Part2	:SuperStrengthHitStar.part
		Part3	:SuperStrengthHitRing.part
		Part4	:SuperStrengthHitRing2.part
		#Part5	:SuperStrengthHitRocks.part
		Sound Rockhitbig 100.0 100.0 .8
	End
End

## PHYSICS DEBRIS ######################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		Type	Local 
		At	chest
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGold.fx
		Lifespan	100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	20

	Event
		Type	Local 
		At	chest
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
		Lifespan	100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local 
		At	chest
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local 
		At	chest
		ChildFX		V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx
		Lifespan	100
	End
End

########################################################

End