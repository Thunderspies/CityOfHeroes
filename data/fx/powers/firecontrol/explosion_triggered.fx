#############################################################
## Explosion_Triggered.fx
#############################################################

FxInfo
Lifespan	90

#############################################################

Input
	Inpname	Origin
End

#############################################################

Condition
	On	Time
	Time	32

	Event
		Type	Local
		At	Origin
		Sound FireZap2 80 80 .5
	End
End

Condition
	Event
		EName 	Prime
		Type	Local
		At	Origin
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
		lifespan	90
	End

	Event
		EName   RadiusFxScale2
		Type	start
		At	chest

		LifeSpan	25
		Part	:SmokeExplosion.part
		Part	:fireExplosion2.part
	End

	Event
		EName 	Primex
		Type	start
		At	chest

		Part2	:TimeBombInferno.part
		Sound 	inferno4 80 80 .9

	End

	Event
		EName 	Primex
		Type	start
		At	chest
		LifeSpan	10
		Part1	:TimeBombBits.part

	End

	Event
		EName 	Primex
		Type	start
		At	Root
		Bhvr	behaviors\CameraShake01_Strong.bhvr
		Lifespan 5
	End

	Event
		EName   RadiusFxScale
		Type	start
		At	chest

		Bhvr	:Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	start
		At	chest
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part

	End

	Event
		EName 	Prime8
		Type	start
		At	chest
		Part1   powers/firecontrol/Infernoflash.part

	End
End


Condition
	On 	time
	Time 	0
	Repeat  10

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	time
	Time 	0
	Repeat  6

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


Condition
	On 	Time
	Time	22

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	32

	Event
		EName 	Prime1
		Type	Destroy
	End

End

End
