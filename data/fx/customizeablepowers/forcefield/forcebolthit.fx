#############################################################
## Tintable_ForceBoltHit.fx
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
		Bhvr	CustomizeablePowers\ForceField\ForceBoltHit.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		Sound ForceBolt 80 80 .8
		Lifespan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		ENAME	BubbleExplosion
		Type	start
		At	HIPS
		Part3	CustomizeablePowers\ForceField\BubbleSphereExplosionRingx.part
		Part4	CustomizeablePowers\ForceField\BubbleSphereExplosionRingz.part
		Part5	CustomizeablePowers\ForceField\BubbleSphereExplosionRingz2.part
		Part	CustomizeablePowers\ForceField\BubbleSphereExplosionRingz3.part
	End
End

Condition
	On 	Time
	Time 	9

	Event
		ENAME	BubbleExplosion
		Type	start
		At	HIPS
		Part1	CustomizeablePowers\ForceField\BubbleSphereExplosion.part
		Part2	CustomizeablePowers\ForceField\BubbleSphereExplosion2.part
	End
End

#############################################################

End