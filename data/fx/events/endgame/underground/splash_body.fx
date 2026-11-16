#############################################################
## Sparks_Body.fx
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On	Cycle
	Time	5
	Chance	0.8
	Random	1

	Event
		EName	Slime1
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 2 0.0
		End
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime1
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime2
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.0 2.0 -0.0
		End
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime3
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime4
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime3
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

	Event
		EName	Slime4
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		CHILDFX	:Geyser_hydrant.fx
		Lifespan 	10
		LifespanJitter	5
	End

End


#############################################################

End