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
		#Sound	Mercenary_Grenade 120.0 30.0 1.0
		sound	shotgun5 120.0 30.0 .9
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
		Type	Create
		Inherit	ALL
		At	T_Root
		Bhvr	powers\firecontrol\Inferno.bhvr
		Sound webgrexplo3 80 80 .8
	End

	Event
		EName	Swarm
		Type	Create
		Inherit	ALL
		At	T_root
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands2.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands3.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands4.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands4.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands4.part
		Part	V_COV\EnemyPowers\Arachnos\BaneMace\WebBurstStrands4.part
		Lifespan 10
	End

	Event
		EName 	Prime1
		Type	start
		At	RadiusFxScale
		bhvr    WEAPONs\Bow\TrickArrow\stickyBubble.bhvr
		Part	WEAPONs\Bow\TrickArrow\StickyYellow2.part
		Part	WEAPONs\Bow\TrickArrow\StickyGlows.part
		LifeSpan 30
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End