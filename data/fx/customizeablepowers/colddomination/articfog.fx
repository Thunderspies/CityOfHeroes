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
		ChildFX	:ColdHandsBoth.fx
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
		bhvr	:Arcticspin.bhvr
		BhvrOverride
			Alpha		35
			TintGeom	1
		End
		geom	Tintable_AirFunnel02
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
		bhvr	:Arcticspin.bhvr
		Part	:WindClouds.part
		Part	:WindClouds2.part
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
		Part1	:O2BoostMistDownFlat.part
		Part2	:O2RingExpand.part
		PMAGNET	Root
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
		Part	:WindClouds.part
		Part	:WindClouds2.part
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