#########################################################
## Umbral Blast - Unchain Essence
#########################################################

FxInfo

LifeSpan	200

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## HANDS ######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local 
		At	WepR
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		Sound WarShadeCorpseBomb 80 80 .53
		LifeSpan	75
	End

	Event
		Type	local 
		At	WepL
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		LifeSpan	75
	End
End

## HANDS ######################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	start 
		At	Chest
		Bhvr	:SoulTransfer2.bhvr
		Magnet	T_chest
		Lookat	T_chest
		Part	:PetrifingPuddleTrail.Part
		part	:Blast2.part
		part	:BlastPurple.part
		part	:BlastBlack.part
		Part	:aFearTenticles.part
		Part	:aFearTenticlesBlue.part
	End

	Event
		EName 	Prime1
		Type	start
		At	Chest
		Bhvr	:TitanRocket1.bhvr
		Magnet	T_chest
		LookAt	T_chest
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime1
		Bhvr	:TitanRocketOffset.bhvr
		Part	:soultrail.part
	End

	Event
		EName 	Prime2
		Type	start
		At	root
		Bhvr	:TitanRocket1opp.bhvr
		Magnet	T_chest
		LookAt	T_chest
	End

	Event
		EName 	Offset2 
		Type	local
		At	Prime2
		Bhvr	:TitanRocketOffsetopp.bhvr
		Part	:soultrail.part
	End

	Event
		EName 	Prime3
		Type	start
		At	WepL
		Bhvr	:TitanRocketA.bhvr
		Magnet	T_chest
		LookAt	T_chest
	End

	Event
		EName 	Offset3 
		Type	local
		At	Prime3
		Bhvr	:TitanRocketOffsetA.bhvr
		Part	:soultrail.part
	End
End

## AOE FX ######################################################

Condition
	On 	PrimeHit

	Event
		EName	SwordFlash2
		Type	Local
		At	T_root
		Part	:NictusSunFlareHitStreaks.part
		Part	:BlastFlash.part
		Part	:QuickBlastHitStreaks.part
		Part	:QuickBlastHitStreaksBlue.part
		Part	:QuickBlastHitStreaksPurple.part
		Part	:PeaceBlastHitSparks.part
		Part	:BlastFlashTendrils.part
		Part	:BlastFlashTendrilsPurple.part
		Part	:BlastFlashTendrilsBlue.part
		LifeSpan 10
	End

	Event
		EName	SwordFlash2
		Type	start
		At	T_root
		Part	:DarkTarRingsInBLK.Part
		Part	:DarkTarRingsInPURPLE.Part
		Part	:DarkTarRingsInBLUE.Part
		LifeSpan 30
	End

	Event
		ENAME	node
		Type	Local
		At	T_root
		BHVR	:HitSpinFaster.bhvr
		LifeSpan 13
	End

	Event
		Type	Local
		At	node
		BHVR	:HitSpinFasterOpposite.bhvr
		Part	:Spinner.part
		LifeSpan 13
	End

	Event
		Type	Local
		At	node
		BHVR	:HitSpinFaster.bhvr
		Part	:Spinner.part
		LifeSpan 13
	End

	Event
		Type	Local
		At	node
		BHVR	:HitSpinFaster2Opposite.bhvr
		Part	:Spinner.part
		LifeSpan 13
	End
End

## PHYSICS FORCES ######################################################

Condition
	On 	PrimeHit
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		400
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

## CLEAN UP ######################################################

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit
	
	Event
		EName 	Prime3
		Type	Destroy
	End
End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End
End

########################################################

EnD