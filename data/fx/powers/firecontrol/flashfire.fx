#############################################################
## Fire Control - Flashfire
#############################################################

FxInfo

Lifespan 240

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		LifeSpan 65
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		LifeSpan 65
	End
End

#############################################################

Condition
	On	Time
	Time	60

	Event
		Type	start
		At	T_root
		Part4	:aHotFeetBaseFlat.part
		part5	:aHotFeetBaseFlat2.part
		part3	:aHotFeetBaseFlat3.part
	End

	Event
		Type	start
		At	T_root
		ChildFX	:Child_Flashfire.fx
	End
End

Condition
	On	Time
	Time	65

	Event
		EName   splathookup
		Type	local
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	:Inferno.bhvr
		Sound Hotignite3 88 88 .83
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
		EName	Ring
		Type	start
		At	T_root
		Part2	:HotfeetRing4.part
		LifeSpan 140
	End
End

#############################################################

Condition
	On	Time
	Time	65

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		40
			PhysForcePower		300
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

#############################################################

End