#############################################################
## Edit2
#############################################################

FXInfo

Lifespan 80
#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Prime
		Type	Local
		At	Origin
		Bhvr	:WarpIn_Fade.bhvr
		Flags	AdoptParentEntity
	End

	Event
		Ename	PortalFlash
		Type	StartPositOnly
		At	Head
		Part	WORLD\Bases\PortalFlash.part
		Part	WORLD\Bases\PortalFlash2.part
		Part	WORLD\Bases\DataRays.part
		Part	WORLD\Bases\PortalRipples.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	3

	Event
		EName 	BodyGlow
		Type	Local
		At	Root
		ChildFX	WORLD\Bases\GlowBody2.fx
		Lifespan 15
	End

	Event
		EName 	DataRings3
		Type	Local
		At	Head
		Part	WORLD\Bases\DataRings2.part
		Lifespan 20
	End
End


#############################################################

End