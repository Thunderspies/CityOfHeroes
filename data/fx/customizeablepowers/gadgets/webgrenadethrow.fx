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
		Bhvr	CustomizeablePowers\Gadgets\grenadeFade.bhvr
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
		Bhvr	CustomizeablePowers\Gadgets\MorterLobbprojectile2.bhvr
		Part1	CustomizeablePowers\Gadgets\BeanBagDust2.part
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
		Bhvr	CustomizeablePowers\Gadgets\Inferno.bhvr
		Sound webgrexplo3 80 80 .8
	End

	Event
		EName 	Prime1
		Type	Start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\Gadgets\stickyBubble.bhvr
		Part	CustomizeablePowers\Gadgets\StickyYellow2.part
		Part	CustomizeablePowers\Gadgets\StickyGlows.part
		LifeSpan 30
	End

	Event
		EName	Swarm
		Type	Create
		Inherit	ALL
		At	T_root
		Part	CustomizeablePowers\Gadgets\WebBurstStrands.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands2.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands3.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands4.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands4.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands4.part
		Part	CustomizeablePowers\Gadgets\WebBurstStrands4.part
		Lifespan 10
	End
End

#############################################################

End