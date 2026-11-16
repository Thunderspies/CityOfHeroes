#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 300

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
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName	MuzzleFlash
		Type	Start
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_SingleShot.fx
		Lifespan 2
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	PositOnly
		At	C_Chest
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\Muzzle_Smoke01.part
		Lifespan 45
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
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer.fx
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	C_Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer.fx
		Lifespan 15
	End
End

#############################################################

End