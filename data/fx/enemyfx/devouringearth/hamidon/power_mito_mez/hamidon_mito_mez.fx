#########################################################
## Blue Mitochondria - Mez attack
########################################################

FxInfo

LifeSpan	60

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Mez_Core.part
		Part	:Hamidon_Mito_Mez_Twinkles.part
		Sound HamidonElecWindup 80 80 .66
		Lifespan	60
	End		

	Event
		EName	TargetHookup
		Type	Posit
		At	T_Waist
		LookAt	Target
	End
End

########################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	TargetHookup
		Sound HamidonElecBlast 80 80 .74
		Part1	:Chain_BeamFlare01.part
		Part1	:Chain_BeamFlare02.part
	End		

	Event
		Type	Local
		At	Waist
		Part1	:ChargedBolt01.part
		Part1	:ChargedBolt02.part
		Part1	:ChargedBolt_Glow2.part
		POther	TargetHookup
	End		
End

########################################################

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	Waist
		Part1	:ChargedBolt04.part
		Part1	:ChargedBolt06.part
		Part1	:ChargedBolt_Glow2.part
		POther	TargetHookup
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Waist
		Part1	:ChargedBolt04.part
		Part1	:ChargedBolt06.part
		Part1	:ChargedBolt_Glow2.part
		POther	TargetHookup
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Waist
		Part1	:ChargedBolt01.part
		Part1	:ChargedBolt02.part
		Part1	:ChargedBolt_Glow2.part
		POther	TargetHookup
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Waist
		Part1	:ChargedBolt03.part
		Part1	:ChargedBolt05.part
		Part1	:ChargedBolt_Glow1.part
		POther	TargetHookup
		Lifespan	60
	End		
End

########################################################

End