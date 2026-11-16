################x#########################################
##	chill touch hit
FxInfo
LifeSpan	75

############################################################


Condition
	On 	Time
	Time 	30

	Event

		ENAME	skull
		Type	local
		At	head
		Anim	Fx_Skull
		bhvr	CustomizeablePowers\DarkMiasma\LargeSkullalpha.bhvr
		Sound aura4 80 80 .8
		Lifespan 	66

	End

End

Condition
	On 	Time
	Time 	20

	Event

		ENAME	HandR
		Type	local
		At	WepR

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 55

	End

	Event

		ENAME	HandL
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 55
	End


	Event
		EName 	Orb
		Type	Local
		At	head

		part1	CustomizeablePowers\DarkMiasma\FearMist.part
		part2	CustomizeablePowers\DarkMiasma\FearMist2.part
		part2	CustomizeablePowers\DarkMiasma\FearMist3.part
		Part3	CustomizeablePowers\DarkMiasma\FearTenticles.part
		Part4	CustomizeablePowers\DarkMiasma\FearTenticles2.part

	End

End

Condition
	On 	Time
	Time 	30

	Event

		ENAME	geoEyes
		Type	local
		At	head
		Geom	HeadEyes
		bhvr	CustomizeablePowers\DarkMiasma\LargeSkullalpha.bhvr

	End

	Event

		ENAME	LeftEye
		Type	local
		At	geoEyes
		altpiv	1
		Part1	CustomizeablePowers\DarkMiasma\RedDaemonEyes.part
		Part2	CustomizeablePowers\DarkMiasma\RedDaemonPupil.part
		Lifespan 64
	End

	Event

		ENAME	RightEye
		Type	local
		At	geoEyes
		altpiv	2

		Part1	CustomizeablePowers\DarkMiasma\RedDaemonEyes.part
		Part2	CustomizeablePowers\DarkMiasma\RedDaemonPupil.part
		Lifespan 64

	End

End