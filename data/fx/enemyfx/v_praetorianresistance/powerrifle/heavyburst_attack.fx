#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		sound RES_PowerRifleHeavy_FireLong_01 100 100 0.8
		sound RES_PowerRifleHeavy_FireLong_02 100 100 0.8
		sound RES_PowerRifleHeavy_FireLong_03 100 100 0.8
		#Sound	gatgun_medium 120.0 30.0 0.9
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\HeavyHands\Child_HeavyBurst_MuzzleFlash.fx
		Lifespan 41
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	21

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	27

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_Cone.fx
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	51

	Event
		Type	PositOnly
		At	C_Chest
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\Muzzle_Smoke01.part
		Lifespan 45
	End
End

#############################################################

End