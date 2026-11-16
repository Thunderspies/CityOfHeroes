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
		Geom	ChestPivotsBuffToHit
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

#############################################################

End