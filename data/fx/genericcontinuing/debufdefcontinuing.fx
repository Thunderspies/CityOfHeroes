#############################################################
## Continuing Effect - Debuff Defense
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition

	Event
		ENAME	Chestnode
		Type	Local
		At	chest
		Geom	ChestPivotsBuffDef
		Lifespan 30
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDefShieldDown.part
		part	:DeBuffDefChevron01.part
		part	:DeBuffDefChevron03.part
		part	:DeBuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60
	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:BuffDefShieldDown.part
		part	:DeBuffDefChevron01.part
		part	:DeBuffDefChevron03.part
		part	:DeBuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60
	End
End

Condition
	On	Cycle
	Time	120

	Event
		ENAME	Chestnode
		Type	Local
		At	chest
		Geom	ChestPivotsBuffDef
		Lifespan 30
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	3

		part	:BuffDefShieldDown.part
		part	:DeBuffDefChevron01.part
		part	:DeBuffDefChevron03.part
		part	:DeBuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60
	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:BuffDefShieldDown.part
		part	:DeBuffDefChevron01.part
		part	:DeBuffDefChevron03.part
		part	:DeBuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60
	End
End

#############################################################

End