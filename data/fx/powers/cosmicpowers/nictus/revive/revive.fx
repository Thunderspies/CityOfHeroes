#########################################################
## Umbral Aura - Stygian Return
#########################################################

FxInfo

Lifespan 250	

## DRAIN FX (On Targets) #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part	:SoulDrainHitPuddle2.Part
		Part	:SoulDrainHitTendril2.part
		Part	:SoulDrainHitTendrilBlue.part
		Part	:SoulDrainHitTendrilPurple.part	
		Part	Powers/DarknessControl/SoulTransferRings.part
		part	Powers/DarknessControl/DamageHitMist.part

		Lifespan	35
	End
End

## PROJECTILE (Targets to Player) #######################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	start 
		At	origin
		
		Bhvr	behaviors/SoulTransfer1.bhvr
		Magnet	Target
		Lookat	Target
		Part	:PetrifingPuddleTrail.Part
		part	:Blast2.part
		part	:BlastPurple.part
		part	:BlastBlack.part
		Part	:aFearTenticles.part
		Part	:aFearTenticlesBlue.part
		Lifespan 150	
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime1
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
		Magnet	Target
		LookAt	Target
		Lifespan 150	
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime1
		Bhvr	:TitanRocketOffset.bhvr
		Part	:soultrail.part
		Lifespan 150	
	End

##	Event
##		EName 	RocketFlash1
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffset.bhvr
##		#Part	:soultrail.part
##		Lifespan 5
##	End
##
##	Event
##		EName 	Offset2 
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffset2.bhvr
##		#Part	:soultrail.part
##		Lifespan 150	
##	End
End

Condition
	On 	Time
	Time 	45
	
	Event
		EName 	Prime2
		Type	start
		At	WepL
		Bhvr	:TitanRocket1opp.bhvr
		Magnet	Target
		LookAt	Target
		Lifespan 150	
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime2
		Bhvr	:TitanRocketOffsetopp.bhvr
		Part	:soultrail.part
		Lifespan 150	
	End

##	Event
##		EName 	RocketFlash1
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffsetopp.bhvr
##		#Part	:soultrail.part
##		Lifespan 5
##	End
##
##	Event
##		EName 	Offset2 
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffset2opp.bhvr
##		#Part	:soultrail.part
##		Lifespan 150	
##	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime3
		Type	start
		At	WepL
		Bhvr	:TitanRocketA.bhvr
		Magnet	Target
		LookAt	Target
		Lifespan 150	
	End

	Event
		EName 	Offset1 
		Type	local
		At	Prime3
		Bhvr	:TitanRocketOffsetA.bhvr
		Part	:soultrail.part
		Lifespan 150	
	End

##	Event
##		EName 	RocketFlash1
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffsetA.bhvr
##		#Part	:soultrail.part
##		Lifespan 5
##	End
##
##	Event
##		EName 	Offset2 
##		Type	local
##		At	Offset1
##		Bhvr	:TitanRocketOffset2A.bhvr
##		#Part	:soultrail.part
##		Lifespan 150		
##	End
End

##Condition
##	On 	Time
##	Time	60
##
##	Event
##		ENAME   Tendrills
##		Type	Local 
##		At	Target
##		Part	Powers/DarknessControl/ChillOfNightTendril.part
##		part	:HealingHitMist.part
##		Part	:HealingGlow.part
##		LifeSpan	120
##	End
##
##	Event
##		ENAME   Tendrills
##		Type	Local 
##		At	Target
##		Part	:SoulDrainHitTendril2.part
##		Part	:SoulDrainHitTendrilBlue.part
##		Part	:SoulDrainHitTendrilPurple.part	
##		LifeSpan	50
##	End
##	
##	Event
##		Type	Local 
##		At	Target
##		Part1	:LightRaysHit.part
##		Part1	:LightRays.part
##		LifeSpan	30
##	End
##End

##Condition
##	On	PrimeHit
##	Chance	0.9
##
##	Event
##		Type	start
##		At	Target
##		Bhvr	:StartOffset.bhvr
##		Part	:GlowingTendrilEnergy.part
##		Part	:GlowingTendrilEnergyBLK.part
##		Part	:GlowingTendrilEnergyBLUE.part
##		Lifespan	130
##	End
##End
##
##Condition
##	On	PrimeHit
##	Chance	0.9
##
##	Event
##		Type	start
##		At	Target
##		Bhvr	:StartOffset2.bhvr
##		Part	:GlowingTendrilEnergy.part
##		Part	:GlowingTendrilEnergyBLK.part
##		Part	:GlowingTendrilEnergyBLUE.part
##		Lifespan	130
##	End
##
##End
##
##Condition
##	On	PrimeHit
##	Chance	0.9
##
##	Event
##		Type	start
##		At	Target
##		Bhvr	:StartOffset3.bhvr
##		Part	:GlowingTendrilEnergy.part
##		Part	:GlowingTendrilEnergyBLK.part
##		Part	:GlowingTendrilEnergyBLUE.part
##		Lifespan	130
##	End
##
##End
##
##Condition
##	On	PrimeHit
##	Chance	0.9
##
##	Event
##		Type	start
##		At	Target
##		Bhvr	:StartOffset4.bhvr
##		Part	:GlowingTendrilEnergy.part
##		Part	:GlowingTendrilEnergyBLK.part
##		Part	:GlowingTendrilEnergyBLUE.part
##		Lifespan	130
##	End
##
##End
##
##Condition
##	On	PrimeHit
##	Chance	0.9
##
##	Event
##		Type	start
##		At	Target
##		Bhvr	:StartOffset5.bhvr
##		Part	:GlowingTendrilEnergy.part
##		Part	:GlowingTendrilEnergyBLK.part
##		Part	:GlowingTendrilEnergyBLUE.part
##		Lifespan	130
##	End
##End

## CLEAN UP #######################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End
End

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

#########################################################

End