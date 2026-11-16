#########################################################
## Staff of the M.A.G.I. - Defensive Aura (Continuing)
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips
		Sound shackleshit 70 70 .6
		Sound eleccage_loop 70 70 .4
	End
End

## INITIAL HIT #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	:ContinuingFX\Hit_Star01.part
		Part4	:ContinuingFX\Hit_Glow01.part
		Part5	:ContinuingFX\Hit_Rays01.part
		Part	:ContinuingFX\Hit_Rays02.part
		LifeSpan	10
	End
End

## CONTINUING FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HandGlow1
		Type	Local
		At	Head

		Part	:ContinuingFX\GlowSmall01.part
		Part	:ContinuingFX\StarSmall01.part
		Part3	:ContinuingFX\Arc03.part
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ContinuingFX\Glow01.part
		Part2	:ContinuingFX\Star01.part
		Part3	:ContinuingFX\Arc02.part
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Hips
		Part1	:ContinuingFX\Glow01.part
		Part2	:ContinuingFX\Star01.part
		Part3	:ContinuingFX\Arc01.part
	End

	Event
		EName	continuingArcs
		Type	Local
		At	LlegR
		Part	:ContinuingFX\GlowSmall01.part
		Part	:ContinuingFX\StarSmall01.part
		Part3	:ContinuingFX\Arc03.part
	End

	Event
		EName	continuingArcs
		Type	Local
		At	LlegL
		Part	:ContinuingFX\GlowSmall01.part
		Part	:ContinuingFX\StarSmall01.part
		Part3	:ContinuingFX\Arc01.part
	End

	Event
		EName	continuingArcs
		Type	Local
		At	FootL
		Part	:ContinuingFX\GlowSmall01.part
		Part	:ContinuingFX\StarSmall01.part
		Part3	:ContinuingFX\Arc02.part
	End

	Event
		EName	continuingArcs
		Type	Local
		At	FootR
		Part	:ContinuingFX\GlowSmall01.part
		Part	:ContinuingFX\StarSmall01.part
		Part3	:ContinuingFX\Arc03.part
	End
End

## PHYSICS PARTICLES #######################################################

Condition
	On	Time 
	Time	2

	Repeat	6
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Green_ElectricSpark.fx
		Lifespan	90
	End
End

Condition
	On	Cycle 
	Time	7

	Chance	0.5
	
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Green_ElectricSpark.fx
		Lifespan	15
	End
End

Condition
	On	Cycle 
	Time	23

	Chance	0.5
	
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Green_ElectricSpark.fx
		Lifespan	15
	End
End

Condition
	On	Time 
	Time	5

	Repeat	4
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Green_LightningHit.fx
		Lifespan	90
	End
End

Condition
	On	Cycle 
	Time	17

	Chance	0.4
	
	Event
		SoftwareOnly
		EName	Sparkfar
		Type 	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\Green_LightningHit.fx
		Lifespan	15
	End
End

#########################################################

End