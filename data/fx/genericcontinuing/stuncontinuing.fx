#############################################################
## Continuing FX - Stun
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Headnode
		Type	Local
		At	hair
		bhvr	behaviors\genericparticlefade.bhvr
		Geom	HairToHeadAdjustment
	End

	Event
		ENAME	STUNFX
		Type	Local
		At	Headnode
		AltPiv	1
		bhvr	behaviors\genericparticlefade.bhvr
		part	:StunRings01.part
		part	:StunSpark01.part
		part	:StunDots01.part
	End
End

#############################################################

End