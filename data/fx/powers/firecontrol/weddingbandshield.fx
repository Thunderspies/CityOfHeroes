#########################################################
##	PlasmaFireAura
FxInfo

##################################

Condition
	On	Time
	Time	18
	
	Event
		Type	Local
		At	Origin
		Sound ignite3 80 80 .44
	End
End


Condition
	On	Time
	Time	60
	
	Event
		Type	Local
		At	Origin
		Sound weddingband_loop 80 80 .34
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End
End



Condition
	On	Time
	Time	60
	Event
		EName	Pivot
		Type	Local
		At	head
		
		part2	:PlasmaFireCore.part

	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		
		part2	:PlasmaFireCore2.part
		part3	:PlasmaFireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		
		part2	:PlasmaFireCore3.part
		part3	:PlasmaFireCoreTrail2.part
		Part4	:FireCoreSparks2.part
	End
	
	Event
		EName	Chest
		Type	Local
		At	Head
		part1	:PlasmaFireCore4.part
		part2	:PlasmaFireCoreTrail3.part
	End


	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:PlasmaFireCore1.part
		part2	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:PlasmaFireCore1.part
		part2	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	:PlasmaFireCore1Feet.part
		part2	:PlasmaFireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	:PlasmaFireCore1Feet.part
		part2	:PlasmaFireCoreTrailFeet.part
	End

End

Condition

	On 	cycle
	Time 	40
	Chance	1

	Event
		EName   eclipse
		Type	local
		At	Hips
		part1	:PlasmaAura.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	20
	End

	
End

End				