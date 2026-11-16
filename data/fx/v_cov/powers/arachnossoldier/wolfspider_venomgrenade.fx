#############################################################
## Arachnos Soldier - Wolf Spider - Frag Grenade
#############################################################

FxInfo

LifeSpan 200

## FIRE FX ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Blast
		Type	Start
		At	C_Chest
		Geom	Fx_GunBurst
		sound	shotgun5 120.0 30.0 0.9
		Lifespan 3
	End

	Event
		EName 	tracer
		Type	start
		At	C_Chest
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Lifespan 12
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Prime
		Type	start
		At	C_Chest
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	WEAPONFX\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		EName   RadiusFxScale
		Type	Local
		At	T_Root
		ChildFX	:WolfSpider_VenomGrenade_Hit.fx
	End
End

#############################################################

End