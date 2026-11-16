#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	WepR
		sound	RES_PowerCannon_FireHERound 100.0 80.0 .9
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	MuzzleFlash
		Type	Start
		At	C_Chest
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

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
	Time 	35

	Event
		Type	PositOnly
		At	C_Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Muzzle_Smoke01.part
		Lifespan 60
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			TrackRate		6.0
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\SparkTrail.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\XXLargeFireBallHeadCore.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\XXLargeFireBallStreak.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\XXLargeFireBallHead.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\Blaze_smoke.part
		LookAt	Target
		Magnet	Target
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
		Ename	ExplosiveForce
		Type	Start
		At	target
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		EName 	Rings
		Type	Create
		Inherit	Position
		Update	None
		At	T_Chest
		bhvr    behaviors\powers\firecontrol\Infernobubble.bhvr
		Part1	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\FireBallHitRing1.part
		Part1	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\FireBallHitRing1_Additive.part
		Part1	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\FireBallHitRing_Shockwave.part

		Part1	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\FireBallHitRing1B.part
		Part2	ENEMYFX\V_PraetorianResistance\PowerCannon\HERoundExplosion\FireBallHitRing2.part
	End
End

#############################################################

End