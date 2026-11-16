#########################################################
##	Fire_Ball
##
FxInfo


Condition

	Event
		Ename	HitFx
		Type	local 
		At	waist
		
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodePurple.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeBlue.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplode.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/streakSparks.part
		sound	natterhit1 50 50 .5
		LifeSpan	7

	End

	
	Event
		Ename	Crystals
		Type	local 
		At	waist
		Geom	NatterlingHoldCrystals02
		Bhvr	Behaviors/NatterlingHold.bhvr
		Sound glow10b_loop 50 50 .33
	End

	
	Event
		
		Type	local 
		At	waist
		Geom	Naterling_Hold
		Bhvr	Behaviors/NatterlingHold.bhvr

	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	1
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	2
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	3
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End


Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	5
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	6
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	7
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	8
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	9
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.1

	Event
		Type	local 
		At	Crystals
		Altpiv	10
		Part	:CrystalShineRings.part
		Part	:CrystalShine.part
		BHVR	Behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End
	
End

Condition
	On 	death
	
	Event
		Ename	DeathFx
		Type	local 
		At	waist
		
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodePurple.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeBlue.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplode.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/streakSparks.part
		sound	crystalhit 60 50 .6
		LifeSpan	7

	End

End	


End			