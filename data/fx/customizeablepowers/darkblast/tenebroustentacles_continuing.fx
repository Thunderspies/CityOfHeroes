#########################################################
##	chill touch hit
FxInfo

Input
	InpName	root
End

Input
	InpName	WepR
End

Input
	InpName	WepL
End

#LifeSpan	100

##############################################################
##Tenticles01
######################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	root
		Sound tentacle_loop 80 80 .6
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End


Condition
	On	Time
	Time	0


	Event

		ENAME	Base
		Type	posit
		At	root

		Part1	CustomizeablePowers\DarkBlast\TendrilPuddle.part
		Part2	CustomizeablePowers\DarkBlast\TendrilPuddle2.part
		Part3	CustomizeablePowers\DarkBlast\DarkTendrilRing.Part
		Part5	CustomizeablePowers\DarkBlast\DarkTendrilsEmber.part
		Sound grexplo2 80 80 .6
	End

	Event
		EName 	Prime
		Type	Local
		At	Root

		Bhvr	CustomizeablePowers\DarkBlast\ScaleTenticle.bhvr

		Anim	FX_tenticlebase
		#Sound	tentacle_loop 80 33 .6
	End


	Event
		EName 	tenticle_1
		Type	Local
		At	Prime
		AnimPiv Chest
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_B
	End



	Event
		EName 	tenticle_2
		Type	Local
		At	Prime
		AnimPiv Neck
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_3
		Type	Local
		At	Prime
		AnimPiv Head
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End

	Event
		EName 	tenticle_4
		Type	Local
		At	Prime
		AnimPiv Hair
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_5
		Type	Local
		At	Prime
		AnimPiv Waist
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End
End

##############################################################
##Tenticles02
######################################################

Condition
	On	Time
	Time	10

	Event
		EName 	Prime2
		Type	Local
		At	Root

		Bhvr	CustomizeablePowers\DarkBlast\ScaleTenticleOutside.bhvr

		Anim	FX_tenticlebase
	End


	Event
		EName 	tenticle_11
		Type	Local
		At	Prime2

		AnimPiv Chest
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_B
	End

	Event
		EName 	tenticle_21
		Type	Local
		At	Prime2
		AnimPiv Neck
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_31
		Type	Local
		At	Prime2
		AnimPiv Head
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End

	Event
		EName 	tenticle_41
		Type	Local
		At	Prime2
		AnimPiv Hair
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle
	End

	Event
		EName 	tenticle_51
		Type	Local
		At	Prime2
		AnimPiv Waist
		Bhvr	CustomizeablePowers\DarkBlast\TentacleFade.bhvr
		Anim	FX_darktenticle_A
	End

End


End