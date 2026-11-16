#########################################################
## Vanguard "Redding" Rifle - Psi-Bomb (Explosion)
#########################################################

FxInfo
LifeSpan	300

## EXPLOSION #######################################################

Condition

	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Root
		Part2	V_COV\Powers\PsychicEnergyBlast\Inferno.part
	End
End

Condition
	On 	Time
	Time 	3
	Event
		EName 	rings
		Type	Local 
		At	Root
		Part1   V_COV\Powers\PsychicEnergyBlast\Infernoflash.part
		Part2   V_COV\Powers\PsychicEnergyBlast\Infernoflash2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Primex
		Type	Local 
		At	Root
		Part1	V_COV\Powers\PsychicEnergyBlast\InfernorockBits.part
		Part2	V_COV\Powers\PsychicEnergyBlast\InfernorockBits2.part
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		BHVR    behaviors\powers\firecontrol\Infernobubble.bhvr
		Part1	V_COV\Powers\PsychicEnergyBlast\InfernoYellow2.part
		Part2	V_COV\Powers\PsychicEnergyBlast\InfernoYellowFlame01.part
	End		
End

Condition
	On 	Time
	Time 	7

	Event
		EName	ring
		Type	Local
		At	RadiusFxScale
		BHVR	V_COV\Powers\PsychicEnergyBlast\BlastRingScale2a.bhvr
		geom	PsychicRing2
		LifeSpan		2
	End
End

## CLEAN UP #######################################################

Condition
	On 	Time
	Time	37

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime1
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End
End

#########################################################

End