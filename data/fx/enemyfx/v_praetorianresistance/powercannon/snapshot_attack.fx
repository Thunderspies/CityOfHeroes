#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		sound	RES_PowerCannon_FireSnapshot 100.0 80.0 .9
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName	MuzzleFlash
		Type	Start
		At	C_Chest
	End
End

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	MuzzleFlash
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerCannon\Child_SingleShot.fx
		Lifespan 4
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	PositOnly
		At	C_Chest
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Muzzle_Smoke01.part
		Lifespan 60
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	9

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			TrackRate		6.0
		End
		LookAt	Target
		Magnet	Target
	End

	Event
		Ename	Prime3
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			PYRRotate	90 0 0
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail03.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\BulletTrail_Trail03b.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End
End

#############################################################

End