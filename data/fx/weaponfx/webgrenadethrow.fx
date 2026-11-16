#############################################################
## webGrenadeThrow.fx
#############################################################

FxInfo

ClampMaxScale 1 1 1

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound grtoss1 60 60 .7
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	behaviors\grenadeFade.bhvr
		Geom	grenade
		lifespan 22
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	Start
		Bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Geom	grenade
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