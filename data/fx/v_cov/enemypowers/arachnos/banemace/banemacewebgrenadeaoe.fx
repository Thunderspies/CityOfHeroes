#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 1 1 1

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	heavyMace
	End

	Event
		Type	Start
		At	Blast
		Altpiv	2
		Geom	Fx_GunBurst
		Bhvr	Behaviors\ShotGun.bhvr
		LifeSpan  2
	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	2
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Part1	WEAPONFX\Gunfire_ShotgunMuzzflash.part
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		sound	grenade2 90.0 30.0 .78
		Lifespan 10
	End

	Event
		EName 	Prime
		Type	start
		At	Blast
		Altpiv	2
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WeaponFx\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

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
		Part	:WebBurstStrands.part
		Part	:WebBurstStrands2.part
		Part	:WebBurstStrands3.part
		Part	:WebBurstStrands4.part
		Part	:WebBurstStrands4.part
		Part	:WebBurstStrands4.part
		Part	:WebBurstStrands4.part
		Lifespan 10
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End