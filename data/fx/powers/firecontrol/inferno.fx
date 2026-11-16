#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	70

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .8

		LifeSpan	70
	End


End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Local
		At	hips


		Part2	powers/fireControl/Inferno.part
		Sound inferno4 100 100 .9

	End

End

Condition
	On 	Time
	Time	57
	Repeat  11

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
	On 	Time
	Time	57
	Repeat  11

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName 	Primex
		Type	Local
		At	hips
		Bhvr	Behaviors/CameraShake01.bhvr
		Part1	powers/firecontrol/InfernorockBits.part
		Part2	powers/fireControl/InfernoExplosion.part
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End
Condition
	On 	Time
	Time 	56

	Event
		EName   RadiusFxScale2
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End

	Event
		EName 	Prime8
		Type	Local
		At	RadiusFxScale2

		Bhvr	:Infernosplat.bhvr
		splat	electricring1copy.tga
#		Part1   powers/firecontrol/Infernoflash.part
		LifeSpan	20
	End
End

Condition
	On 	Time
	Time	47

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
	Time	57

	Event
		EName 	Prime1
		Type	Destroy
	End

End

End
