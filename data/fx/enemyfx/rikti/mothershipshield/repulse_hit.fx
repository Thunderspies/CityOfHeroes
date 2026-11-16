#########################################################
## Rikti Mothership Shield - Repulsion Hit FX
########################################################

FxInfo

LifeSpan 30

########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	BubbleHit
		Type	start 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBoltHit.bhvr
		Geom	FX_ForceBubble
		#Part1	POWERS\ForceField\BubbleSphere3.part
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
		Part3	POWERS\ForceField\BubbleSphereExplosionRingx.part
		Part4	POWERS\ForceField\BubbleSphereExplosionRingz.part
		Part5	POWERS\ForceField\BubbleSphereExplosionRingz2.part
		Part	POWERS\ForceField\BubbleSphereExplosionRingz3.part
	End
End

Condition
	On 	Time
	Time 	9

	Event

		ENAME	BubbleExplosion
		Type	start 
		At	HIPS
		Part1	POWERS\ForceField\BubbleSphereExplosion.part
		Part2	POWERS\ForceField\BubbleSphereExplosion2.part
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

########################################################

End