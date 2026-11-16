#############################################################
## Peacebringer - Shining Shield
#############################################################

FxInfo
	
## SOUND FX ###########################################################

Condition
	On	Time	
	Time 	1

	Event
		Type 	Local
		At	Origin
		Sound glow6_loop 70.0 70.0 .44
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Head
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames1.part
	End

	Event
		Type	Local
		At	UArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames1.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames1.part
		pOther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames1.part
		pOther	HandL
	End

	Event
		Type	Local
		At	LArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames1.part
		pOther	HandR
	End

	Event
		Type	Local
		At	ULegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames2.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames2.part
		pOther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames2.part
		pOther	FootL
	End

	Event
		Type	Local
		At	LLegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:CosmicFlames2.part
		pOther	FootR
	End
End

## RICOCHETS ###########################################################

Condition
	On 	cycle
	Time 	20
	Chance	0.125
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.125
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.125
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.125
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:Ricochet_Rays.part
		part2	:Ricochet_Ring.part
		LifeSpan	25
	End
End

#############################################################

End