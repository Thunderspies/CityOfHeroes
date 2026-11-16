#############################################################
## ForceBoltHit.fx
#############################################################

FxInfo
LifeSpan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################


Condition
	On 	Time
	Time 	0

	Event
		ENAME	BubbleHit
		Type	start
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBoltHit.bhvr

		Geom	FX_ForceBubble
		#Part1	:BubbleSphere3.part
		Sound forcebolt 80 80 .8
	End

End

Condition
	On 	Time
	Time 	5

	Event

		ENAME	BubbleExplosion
		Type	start
		At	HIPS

		Part3	:BubbleSphereExplosionRingx.part
		Part4	:BubbleSphereExplosionRingz.part
		Part5	:BubbleSphereExplosionRingz2.part
		Part	:BubbleSphereExplosionRingz3.part

	End

End

Condition
	On 	Time
	Time 	9

	Event

		ENAME	BubbleExplosion
		Type	start
		At	HIPS

		Part1	:BubbleSphereExplosion.part
		Part2	:BubbleSphereExplosion2.part

	End

End

Condition
	On 	Time
	Time 	10
	Event
		Type	Destroy
		ENAME	BubbleHit

	End

End

End
