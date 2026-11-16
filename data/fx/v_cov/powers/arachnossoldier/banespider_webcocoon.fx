#############################################################
## Arachnos Soldier - Bane Spider - Web Cocoon (Fired from Mace)
#############################################################

FxInfo

LifeSpan 200

## FIRE ###########################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Blast
		Type	Local
		At	WepR
		Geom	heavyMace
	End

	Event
		Type	Start
		At	Blast
		Altpiv	2
		Bhvr	Behaviors\ShotGun.bhvr
		Geom	Fx_GunBurst
		LifeSpan  3
	End

	Event
		Type	Start
		At	Blast
		Altpiv	2
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	grenade4 80.0 30.0 .8
		LifeSpan  10
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	start
		At	Blast
		Altpiv	2
		Bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WeaponFx\BeanBagDust2.part
		Geom	grenade
		Magnet	T_Root
		LookAt	T_Root
	End
End

## ON HIT ###########################################################

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
		EName 	Prime1
		Type	Start
		At	RadiusFxScale
		bhvr    WEAPONs\Bow\TrickArrow\stickyBubble.bhvr
		Part	WEAPONs\Bow\TrickArrow\StickyYellow2.part
		Part	WEAPONs\Bow\TrickArrow\StickyGlows.part
		LifeSpan 30
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
End

#############################################################

End