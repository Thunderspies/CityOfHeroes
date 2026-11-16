#########################################################
## Continuing FX - Buff Defense
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#########################################################

Condition

	Event
		ENAME	Chestnode
		Type	Local
		At	hips

		bhvr	Behaviors\ContinuingFXspinSlow.bhvr
		Geom	ChestPivotsBuffToHit
		Lifespan 30
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

End

Condition
	On	Cycle
	Time	90


	Event
		ENAME	Chestnode
		Type	Local
		At	hips

		bhvr	Behaviors\ContinuingFXspinSlow.bhvr
		Geom	ChestPivotsBuffToHit
		Lifespan 30
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffDefShield.part
		part	:BuffDefChevron01.part
		part	:BuffDefChevron03.part
		part	:BuffDefChevron02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60

	End

End


End