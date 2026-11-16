FXInfo

	Condition
		On	Cycle
		Time	1
		Chance	0.7
		Random	1

		Event
			EName	Dripsa
			Type	PositOnly
			At	Head
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 5
		End

		Event
			EName	Drips1
			Type	PositOnly
			At	UArmL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 5
		End

		Event
			EName	Drips2
			Type	PositOnly
			At	UArmR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 5
		End

		Event
			EName	Drips3
			Type	PositOnly
			At	LArmL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips4
			Type	PositOnly
			At	LArmR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips5
			Type	PositOnly
			At	HandL
			Bhvr	behaviors/GenericParticleFade.bhvr
		#	Part2	:Drips_Hands.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips6
			Type	PositOnly
			At	HandR
			Bhvr	behaviors/GenericParticleFade.bhvr
		#	Part2	:Drips_Hands.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips7
			Type	PositOnly
			At	ULegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips8
			Type	PositOnly
			At	ULegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	Drips9
			Type	Local
			Bhvr	behaviors/GenericParticleFade.bhvr
			At	Chest
			Part2	:Drips_Chest.part
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
			Part	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	positonly
			At	UArmR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	local
			At	LLegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	local
			At	LLegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End


		Event
			EName	AOERing
			Type	positonly
			At	HandL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	positonly
			At	HandR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips_Arms.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	positonly
			At	LLegR
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips_Foot.part
			Lifespan 	5
			LifespanJitter 	2
		End

		Event
			EName	AOERing
			Type	positonly
			At	LLegL
			Bhvr	behaviors/GenericParticleFade.bhvr
			Part2	:Drips_Foot.part
			Lifespan 	5
			LifespanJitter 	2
		End
	End

	Condition
		On Cycle
		Time 5
		Repeat 1
		RepeatJitter 1

		Event
			At Root
			Type Start
			EName Splashes
			BhvrOverride
				StartJitter 2 0 1
			End
			Part :RainDrop_Splash_Small.part
			LifeSpan 1
		End
	End

End