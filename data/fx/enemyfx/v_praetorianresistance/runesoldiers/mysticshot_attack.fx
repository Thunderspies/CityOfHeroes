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
		Type	Local
		At	WepR
		sound RES_PowerRifleHeavy_FireSingle_01 100.0 100.0 0.8
		sound RES_PowerRifleHeavy_FireSingle_02 100.0 100.0 0.8
		sound RES_PowerRifleHeavy_FireSingle_03 100.0 100.0 0.8
		LifeSpan 5
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
		ChildFX	:Child_SingleShot.fx
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

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	8

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			TrackRate		6.0
		End
		LookAt	Target
		Magnet	Target
		Lifespan 300
	End

	Event
		Ename	Incendiary1
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	:BulletTrail_Round01.part
		Part	:BulletTrail_Round02.part
		Lifespan 5
	End

	Event
		Ename	Incendiary2
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	:BulletTrail_Trail01.part
		Part	:BulletTrail_Trail03.part
		Lifespan 10
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Incendiary1
		Type	Destroy
	End

	Event
		Ename	Incendiary2
		Type	Destroy
	End
End

#############################################################

End