#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	wepR
		geom	grenade
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	:Grenade_Streak.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

Condition
	On 	PrimeHit

	Event
		EName	Prime
		Type	DESTROY
	End

	Event
		Type	Local
		At	T_Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:Explosion_Glow_Lingering_Flat.part
		Part	:Explosion_Shockwave_Flat.part
		Part	:Explosion_Flash_Large_Flat.part

		Lifespan 5
	End

	Event
		Type	Local
		At	T_Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Streaks_Thin_Small.part
		Part	:Explosion_Sparks_Small.part
		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Explosion_Smoke.part
		Part	:Explosion_Smoke_Thin.part
		Part	:Explosion_Fire_Rings_Small.part
		Part	:Explosion_ElectricArcs.part
		Lifespan 10
	End
End

#############################################################
