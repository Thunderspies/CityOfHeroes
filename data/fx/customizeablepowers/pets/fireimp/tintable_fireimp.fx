#############################################################
## FireImp.fx
#############################################################

FxInfo

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	Geometry
#		Type	Local
#		At	Root
#		Anim	Pet_FireImp_ColorTintable
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	:Inferno.part
		Part3	:InfernoExplosion.part
		Part2	:InfernorockBits.part
		Lifespan 10
	End
End

## EYES ###########################################################

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
		Type	Local
		At	core
		Altpiv	1
		Part1	:elementalEyeGlow.part
		Part2	:elementalEyePupil.part
		Part3	:elementalEyeGlowstars.part
	End

	Event
		Type	Local
		At	core
		Altpiv	2
		Part1	:elementalEyeGlow.part
		Part2	:elementalEyePupil.part
		Part3	:elementalEyeGlowstars.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		Part1	:FireElementalBack.part
		Sound firecrackle_loop 64 64 .24
	End

	Event
		Type	Local
		At	WepR
		Part1	:FireElementalHandz.part
	End

	Event
		Type	Local
		At	WepL
		Part1	:FireElementalHandz.part
	End

	Event
		Type	Local
		At	hips
		Part1	:FireElementalFire2.part
	End

	Event
		Type	Local
		At	Head
		Part1	:FireElementalFire.part
	End

	Event
		Type	Local
		At	Neck
		Part1	:FireElementalFire.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	UarmL
		Part1	:FireElementalFireLegs2.part
		Pmagnet LArmL
	End

	Event
		Type	Local
		At	UarmR
		Part1	:FireElementalFireLegs2.part
		Pmagnet LArmR
	End

	Event
		Type	Local
		At	LArmL
		Part1	:FireElementalFirearm2.part
		Pmagnet WepL
	End

	Event
		Type	Local
		At	LArmR
		Part1	:FireElementalFirearm2.part
		Pmagnet WepR
	End

	Event
		Type	Local
		At	LlegL
		Part1	:FireElementalFireLegsTop.part
		Pmagnet UlegL
	End

	Event
		Type	Local
		At	LlegR
		Part1	:FireElementalFireLegsTop.part
		Pmagnet UlegR
	End

	Event
		Type	Local
		At	FootL
		Part1	:FireElementalFireLegs.part
		Pmagnet LlegL
	End

	Event
		Type	Local
		At	FootR
		Part1	:FireElementalFireLegs.part
		Pmagnet LlegR
	End
End

#############################################################

End