#############################################################
## Blood Widow - Small Hit FX
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Chest
		Part1	powers\Claws\Clawflash.part
		Part3	powers\Claws\ClawEmbers.part
		Part4	powers\Claws\ClawEmbersBroad.part
		Part5	powers\Claws\ClawsStar.part
		Sound spinhit 100.0 100.0 .7
		Lifespan 2
	End

	Event
		Type	Local
		At	Chest
		ChildFX	:Child_PoisonHit_Small.fx
	End
End

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
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End

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