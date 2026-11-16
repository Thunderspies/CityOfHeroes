#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Chestnode
		Type	Local
		At	Origin
		Geom	ChestPivotsBuffHP
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:DeBuffHPRingFlash.part
		part	:DeBuffHPRing01.part
		part	:DeBuffHPRing02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 20
	End
End

#############################################################

End