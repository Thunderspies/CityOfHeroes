#############################################################
## HamiDeath_Hit.fx
#############################################################

FxInfo
#Lifespan 200
#############################################################

Condition
	On	Cycle
	Time	1
	Chance	0.7
	Random	1

	Event
		EName	Slime1
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 5
	End

	Event
		EName	Slime1
		Type	PositOnly
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 5
	End

	Event
		EName	Slime2
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 5
	End

	Event
		EName	Slime3
		Type	PositOnly
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime4
		Type	PositOnly
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime3
		Type	PositOnly
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
	#	Part2	:Slime_Hands.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime4
		Type	PositOnly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
	#	Part2	:Slime_Hands.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime5
		Type	PositOnly
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	Slime6
		Type	PositOnly
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	Chest
		Part2	:Slime_Chest.part
		Lifespan 	5
		LifespanJitter 	2
	End
End

Condition
	On	Cycle
	Time	2
	Random	1

	Event
		EName	AOERing
		Type	Positonly
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	positonly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End


	Event
		EName	AOERing
		Type	positonly
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	positonly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Arms.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	positonly
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Foot.part
		Lifespan 	5
		LifespanJitter 	2
	End

	Event
		EName	AOERing
		Type	positonly
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part2	:Slime_Foot.part
		Lifespan 	5
		LifespanJitter 	2
	End
End
############################################################
End