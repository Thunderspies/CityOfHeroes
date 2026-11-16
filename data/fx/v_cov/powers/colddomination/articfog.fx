#########################################################
## Cold Domination - Arctic Fog
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	root
		bhvr	behaviors\soundFade3.bhvr
		Sound fog_loop 80 80 0.7
		Lifespan 220
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	root
		Sound fog_loop 80 80 0.1
	End
End

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	V_COV\Powers\ColdDomination\ColdHandsBoth.fx
		Lifespan 60
	End
End

## AOE FX #######################################################

Condition
	On	cycle
	Time	10

	Event
		Ename   targetchill
		Type	Local
		At	Root
		bhvr	behaviors\Arcticspin.bhvr
		geom	AirFunnel02
		LifeSpan 10
	End
End

Condition
	On	cycle
	Time	15

	Event
		Ename   targetchill
		Type	Local
		At	Root
		bhvr	behaviors\Arcticspin.bhvr
		Part3	:WindClouds.part
		Part3	:WindClouds2.part
		LifeSpan 10
	End
End

Condition
	On	cycle
	Time	15

	Event
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	Root
		Part1	:O2BoostMistDownFlat.part
		Part2	:O2RingExpand.part
		LifeSpan 30
	End
End

Condition
	On	cycle
	Time	15

	Event
		Ename   targetchill
		Type	Local
		At	neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:WindCloudsDown.part
		Part	:WindClouds2Down.part
		LifeSpan 10
	End
End

Condition
	On	cycle
	Time	30

	Event
		Ename   targetchill
		Type	Local
		At	Root
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:WindCloudsa.part
		Part	:WindClouds2a.part
		LifeSpan 30
	End
End

## PHYSICS FORCES #######################################################

Condition

	On	Time
	Time	10

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		30
			PhysForcePowerJitter	10
		End
	End
End

#########################################################

End