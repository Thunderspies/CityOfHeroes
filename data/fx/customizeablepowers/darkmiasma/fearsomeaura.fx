################x#########################################
##	chill touch hit
FxInfo

Input
	InpName	head
End

Input
	InpName	WepR
End

Input
	InpName	WepL
End

Input
	InpName	Chest
End

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
		bhvr	CustomizeablePowers\DarkMiasma\SkullAlpha.bhvr
		Sound aura4 80 80 .8
		Lifespan 34
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
		Lifespan 35

	End

	Event

		ENAME	HandL
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 35

	End


	Event
		EName 	Orb
		Type	Local
		At	head

		part1	CustomizeablePowers\DarkMiasma\FearMist.part
		part2	CustomizeablePowers\DarkMiasma\FearMist2.part
		Part3	CustomizeablePowers\DarkMiasma\FearTenticles.part
		Part4	CustomizeablePowers\DarkMiasma\FearTenticles2.part
		Lifespan 35


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
		bhvr	CustomizeablePowers\DarkMiasma\SkullAlpha.bhvr

	End

	Event

		ENAME	LeftEye
		Type	local
		At	geoEyes
		altpiv	1
		Part1	CustomizeablePowers\DarkMiasma\DaemonEyes.part
		Part2	CustomizeablePowers\DarkMiasma\DaemonPupil.part
		Lifespan 32
	End

	Event

		ENAME	RightEye
		Type	local
		At	geoEyes
		altpiv	2
		Part1	CustomizeablePowers\DarkMiasma\DaemonEyes.part
		Part2	CustomizeablePowers\DarkMiasma\DaemonPupil.part
		Lifespan 32

	End

End





End