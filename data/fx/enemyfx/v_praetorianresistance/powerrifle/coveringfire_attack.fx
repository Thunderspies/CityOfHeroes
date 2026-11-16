#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		sound RES_PowerRifleHeavy_FireShort_01 100 100 0.8
		sound RES_PowerRifleHeavy_FireShort_02 100 100 0.8
		sound RES_PowerRifleHeavy_FireShort_03 100 100 0.8
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
		Lifespan 17
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		Lifespan 60
	End
End

#############################################################

Condition
	On 	Time
	Time 	24

	Event
		Type	PositOnly
		At	C_Chest
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\Muzzle_Smoke01.part
		Lifespan 45
	End
End

#############################################################

End