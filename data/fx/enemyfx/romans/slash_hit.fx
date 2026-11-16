#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 70

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	Powers\Claws\Clawflash.part
		Part3	Powers\Claws\ClawEmbers.part
		Part4	Powers\Claws\ClawEmbersBroad.part
		Part5	Powers\Claws\ClawsStar.part
		Lifespan 2
	End
End
#############################################################
Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Sound1
		Type	Start
		At	origin
		Sound Roman_soldier_sword_impact_1 80.0 80.0 .9
	End
	
	Event
		EName	Sound2
		Type	Start
		At	origin
		Sound Roman_soldier_sword_impact_2 80.0 80.0 .9
	End
	
	Event
		EName	Sound1
		Type	Start
		At	origin
		Sound Roman_soldier_sword_impact_3 80.0 80.0 .9
	End
	
	Event
		EName	Sound2
		Type	Start
		At	origin
		Sound Roman_soldier_sword_impact_4 80.0 80.0 .9
	End

	Event
		EName	Sound2
		Type	Start
		At	origin
		Sound Roman_soldier_sword_impact_5 80.0 80.0 .9
	End

End
#############################################################

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter	3

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	3
	RepeatJitter	1
	Chance	0.5

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUTbig.fx
	End
End

Condition
	On	Time
	Time	2
	Repeat	4
	RepeatJitter	2
	Chance	0.75

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

#############################################################

End