#############################################################
## Blood Widow "Strike" Hit FX
#############################################################

FxInfo

LifeSpan 50

## FIRST HIT ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Chest
		Part1	powers\Claws\Clawflash.part
		Part3	powers\Claws\LightClawEmbers.part
		Part4	powers\Claws\LightClawEmbersBroad.part
		Part5	powers\Claws\ClawsStar.part
		Sound EviscerHit2 100.0 100.0 .8
		Lifespan 2
	End

	Event
		Type	Local
		At	Chest
		ChildFX	:Child_PoisonHit_Small.fx
	End
End

## FIRST HIT ###########################################################

Condition
	On	Time
	Time	0
	Repeat	1

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	1

	Event
		EName	Sparkdown
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkUP.fx
	End

	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkExplodeUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	1
	Chance	0.5

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUTbig.fx
	End
End

Condition
	On	Time
	Time	2
	Repeat	1
	Chance	0.75

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightMetalSparkOUT.fx
	End
End

#############################################################

End