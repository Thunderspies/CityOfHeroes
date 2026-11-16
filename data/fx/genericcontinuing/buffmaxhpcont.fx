#########################################################
## Continuing FX - Buff Max HP
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#########################################################

Condition

	Event
		ENAME	Chestnode
		Type	Local
		At	chest
		Geom	ChestPivotsBuffHP
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:BuffHPRingFlash.part
		part	:BuffHPRing01.part
		part	:BuffHPRing02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 20

	End

End

Condition
	On	Cycle
	Time	120

	Event
		ENAME	Chestnode
		Type	Local
		At	chest
		Geom	ChestPivotsBuffHP
		Lifespan 20
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:BuffHPRingFlash.part
		part	:BuffHPRing01.part
		part	:BuffHPRing02.part
		Bhvr	\Behaviors\GenericParticleFade.bhvr
		Lifespan 20

	End

End


End