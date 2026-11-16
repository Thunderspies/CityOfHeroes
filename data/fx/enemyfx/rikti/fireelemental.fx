#############################################################
## FireElemental.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	spawnFire
		Type	Local
		At	Chest
		Part1	Powers\FireControl\Inferno.part
		Part2	powers\firecontrol\InfernorockBits.part
		Part3	powers\fireControl\InfernoExplosion.part
		Lifespan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	Rikti_MonkeyEyes
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		Part1	Powers\FireControl\elementalEyeGlow.part
		Part2	Powers\FireControl\elementalEyePupil.part
		Part3	Powers\FireControl\elementalEyeGlowstars.part
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	2
		Part1	Powers\FireControl\elementalEyeGlow.part
		Part2	Powers\FireControl\elementalEyePupil.part
		Part3	Powers\FireControl\elementalEyeGlowstars.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	Back
		Part1	Powers\FireControl\FireElementalBack.part
		Sound firecrackle_loop 64 64 .24
	End

	Event
		EName	Chest
		Type	Local
		At	UarmR
		Part1	Powers\FireControl\FireElementalFireLegs2.part
		Pmagnet LArmR
	End

	Event
		EName	Head
		Type	Local
		At	LArmR
		Part1	Powers\FireControl\FireElementalFirearm2.part
		Pmagnet WepR
	End

	Event
		EName	Chest
		Type	Local
		At	UarmL
		Part1	Powers\FireControl\FireElementalFireLegs2.part
		Pmagnet LArmL
	End

	Event
		EName	Head
		Type	Local
		At	LArmL
		Part1	Powers\FireControl\FireElementalFirearm2.part
		Pmagnet WepL
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	Powers\FireControl\FireElementalHandz.part
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	Powers\FireControl\FireElementalHandz.part
	End

	Event
		EName	Chest
		Type	Local
		At	hips
		Part1	Powers\FireControl\FireElementalFire2.part
	End

	Event
		EName	Head
		Type	Local
		At	Head
		Part1	Powers\FireControl\FireElementalFire.part
	End

	Event
		EName	Neck
		Type	Local
		At	Neck
		Part1	Powers\FireControl\FireElementalFire.part
	End

	Event
		EName	Chest
		Type	Local
		At	FootL
		Part1	Powers\FireControl\FireElementalFireLegs.part
		Pmagnet LlegL
	End

	Event
		EName	Chest
		Type	Local
		At	LlegL
		Part1	Powers\FireControl\FireElementalFireLegsTop.part
		Pmagnet UlegL
	End

	Event
		EName	hipmovement
		Type	Local
		At	UlegL
		Part1	Powers\FireControl\FireElementalFireLegsTop2.part
		Pmagnet	hips
	End

	Event
		EName	hipmovement2
		Type	Local
		At	UlegR
		Part1	Powers\FireControl\FireElementalFireLegsTop2.part
		Pmagnet	hips
	End

	Event
		EName	Head
		Type	Local
		At	FootR
		Part1	Powers\FireControl\FireElementalFireLegs.part
		Pmagnet LlegR
	End

	Event
		EName	Head
		Type	Local
		At	LlegR
		Part1	Powers\FireControl\FireElementalFireLegsTop.part
		Pmagnet UlegR
	End
End

#############################################################

End