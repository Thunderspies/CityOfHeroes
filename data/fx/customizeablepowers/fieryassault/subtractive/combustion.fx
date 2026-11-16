############################################################
##	Fireweapon
###########################################################
FxInfo
Lifespan	90



Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part	CustomizeablePowers\FieryAssault\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	52

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part	CustomizeablePowers\FieryAssault\FireHandz.part
		Sound Ignite2 100.0 100.0 .8

		LifeSpan	52
	End

	Event
		Type	Local
		At	WepL
		Sound Rumble7 100. 100. .73
		End


End

Condition

	On 	Time
	Time 	60

	Event
		EName   eclipse
		Type	local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Sound fireball1 60 60 .77
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Hips
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\FieryAssault\Infernobubble4.bhvr
		Part1	CustomizeablePowers\FieryAssault\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FieryAssault\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FieryAssault\FireShieldSparksB.part

		#LifeSpan	30

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End


##############################################################

