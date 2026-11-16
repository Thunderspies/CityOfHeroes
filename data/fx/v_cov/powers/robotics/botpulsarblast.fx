#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

######## Sound  ###########################

Condition
	On	Time
	Time	30

	Event	
		Type	Local
		At	WepR
		Sound PhotonGrenadeLaunch2 80 80 .9
	End
End

###############################################################################
######## arrow projectile  ##########################

Condition
	On 	Time
	Time 	29

	Event
		Type	Local
		At	WepR
		BHVR	:BlastOffset.bhvr
		Part	:BlastOriginStarBlue.part
		Part	:BlastOriginEnergyBlue.part
#		Part	:BlastOriginEnergyBlue.part
		LifeSpan	12
	End

End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	prime
		Type	start
		At	WepR
		Bhvr	:PhotonProjectile.bhvr

		Part	:ProtonLight.part
		Part	:ProtonLightOpposite.part
		Part	:ProtonWhiteLightRays.part
		Part	:ProtonLightRays.part
		Part	:ProtonDarkRays.part

		Part	:ProtonStar.part
		Part	:ProtonStar.part
		
		Part	:ProtonSquare.part
		Part	:ProtonLightPulse.part

		Magnet	t_Root
		Lookat	t_Root

		LifeSpan	300

	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		Sound PhotonGrenadeExplo 100 100 1.0
		LifeSpan	3
	End

	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End
	
	Event
		EName 	Rings
		Type	local
		At	T_Root

		BHVR	Behaviors/spinrandom.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End

	Event
		Type	Start
		At	T_Root

		Part	:PulsarBlastHitGlowFlat.part
		Part	:PulsarBlastHitGlowStarFlat.part
		Part	:PulsarBlastHitGlow.part
		Part	:PulsarBlastHitGlowStar.part

		Part	:PulsarBlastHitGlow1.part
		Part	:PulsarBlastHitGlow1.part
		Part	:PulsarBlastHitGlow1.part
		Part	:PulsarBlastHitStar.part
		Part	:PulsarBlastHitGlow1Flat.part
		Part	:PulsarBlastHitGlow1Flat.part
		Part	:PulsarBlastHitGlow1Flat.part
		Part	:PulsarBlastHitStarFlat.part
		Part	:PulsarBlastGlowStreaks.part

		LifeSpan	13

	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End

End		