############################################################
##	Fireweapon
###########################################################
FxInfo

Input
	Inpname	WepR
End

Input
	Inpname	WepL
End

Input
	Inpname	Hips
End

Lifespan	90

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FireManipulation\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	52

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FireManipulation\FireHandz.part
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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound fireball1 60 60 .77
		#part1	:FireAuraOut.part
		#LifeSpan	30
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Hips
		#Bhvr	powers\firecontrol\FireShield.bhvr
		#Sound	aura6 50 30 .24
		#Sound	aura5 50 30 .22
		#Sound	aura7 50 30 .19
		#LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\FireManipulation\Infernobubble4.bhvr
		Part1	CustomizeablePowers\FireManipulation\FireShieldCloud01B.part
		Part2	CustomizeablePowers\FireManipulation\FireShieldCloud02B.part
		Part4	CustomizeablePowers\FireManipulation\FireShieldSparksB.part

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

End

##############################################################

