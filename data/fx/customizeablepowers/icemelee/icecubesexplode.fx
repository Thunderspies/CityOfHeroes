#############################################################
## IceCubesExplode.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkDebris.bhvr
		Part	:ColdStars.part
		Part	:IceCrystal.part
		LifeSpan 	75
		LifeSpanJitter 	30
	End
End

#############################################################

End