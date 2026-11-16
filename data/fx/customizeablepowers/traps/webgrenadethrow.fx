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
		Bhvr	CustomizeablePowers\Traps\grenadeFade.bhvr
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
		Bhvr	CustomizeablePowers\Traps\MorterLobbprojectile2.bhvr
		Part1	CustomizeablePowers\Traps\BeanBagDust2.part
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
		Bhvr	CustomizeablePowers\Traps\Inferno.bhvr
		Sound webgrexplo3 80 80 .8
	End

	Event
		EName 	Prime1
		Type	Start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\Traps\stickyBubble.bhvr
		Part	CustomizeablePowers\Traps\StickyYellow2.part
		Part	CustomizeablePowers\Traps\StickyGlows.part
		LifeSpan 30
	End

	Event
		EName	Swarm
		Type	Create
		Inherit	ALL
		At	T_root
		Part	CustomizeablePowers\Traps\WebBurstStrands.part
		Part	CustomizeablePowers\Traps\WebBurstStrands2.part
		Part	CustomizeablePowers\Traps\WebBurstStrands3.part
		Part	CustomizeablePowers\Traps\WebBurstStrands4.part
		Part	CustomizeablePowers\Traps\WebBurstStrands4.part
		Part	CustomizeablePowers\Traps\WebBurstStrands4.part
		Part	CustomizeablePowers\Traps\WebBurstStrands4.part
		Lifespan 10
	End
End

#############################################################

End