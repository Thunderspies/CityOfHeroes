#############################################################
## RobotLazerBursts.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MuzzleFlash_Right
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.5 0 0.625
		End
	End

	Event
		EName	MuzzleFlash_Left
		Type	Local
		At	WepL
		BhvrOverride
			PYRRotate		0 180 0
			PositionOffset		-0.5 0 0.625
		End
	End
End

Condition
	On Time
	Time 5
	
	Event
		Type	Local
		At	WepR
		sound	RES_Pistols_FireOvercharged_01 100.0 80.0 .9
		LifeSpan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	MuzzleFlash_Right
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Overcharge_Glow01.part
		LifeSpan 10
	End

	Event
		Type	Local
		At	MuzzleFlash_Left
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Overcharge_Glow01.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Type	Local
		At	MuzzleFlash_Left
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_LargeMuzzleFlash_01.fx
		LifeSpan 2
	End

	Event
		Type	Local
		At	MuzzleFlash_Right
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_LargeMuzzleFlash_01.fx
		LifeSpan 2
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlash_Right
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Head01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_LargeCore01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_LargeGlow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Ename	Prime2
		Type	Start
		At	MuzzleFlash_Left
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Head01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_LargeCore01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_LargeGlow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	Prime
	End

	Event
		Type	Destroy
		Ename	Prime2
	End

	Event
		Type	Start
		At	Target
		BHVR	Behaviors\spinrandom.bhvr
		Part	V_COV\Powers\Robotics\HitRingflashthin.part
		Part	V_COV\Powers\Robotics\HitRingflashthin2.part
		Part	V_COV\Powers\Robotics\HitRingflash.part
		#Sound	PhotonGrenadeExplo 100 70 1.0
		LifeSpan 3
	End

	Event
		Type	Start
		At	T_Root
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlowFlat.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlowStarFlat.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlowStar.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitStar.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1Flat.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1Flat.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitGlow1Flat.part
		Part	V_COV\Powers\Robotics\PulsarBlastHitStarFlat.part
		Part	V_COV\Powers\Robotics\PulsarBlastGlowStreaks.part
		LifeSpan 13
	End
End

#############################################################

End