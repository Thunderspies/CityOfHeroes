#############################################################
## Edit2
#############################################################

FXInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		Ename	Prime
		Type	Local
		At	Origin
		Bhvr	:WarpOut_Fade.bhvr
		Flags	AdoptParentEntity
		Lifespan 1
	End



	Event
		EName 	DataRings
		Type	Local
		At	Head
		ChildFX	WORLD\Bases\GlowBody.fx
		Lifespan 35
	End


End


Condition
	On	Time
	Time	25

	Event
		EName 	DataRings
		Type	Local
		At	Head
		Part	WORLD\Bases\DataRings.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time 	40

	Event
		Ename	PortalFlash
		Type	StartPositOnly
		At 	Head
		Part	WORLD\Bases\PortalFlash2.part
		Part	WORLD\Bases\DataRays.part
		Part	WORLD\Bases\PortalFlash.part
		Part	WORLD\Bases\PortalRipples.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	30	#89

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Bhvr	behaviors\invisible.bhvr
		Flags	AdoptParentEntity
	End
End




#############################################################

End