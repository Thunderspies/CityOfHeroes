#########################################################
##	template

FxInfo

################################
##oIL bURST

Condition

	Event
		EName	spinny
		Type	Posit
		At	root
		Geom	Centerdummy
		Bhvr	Behaviors/Splat/spinnyOffset.bhvr
	End
	
	Event
		EName	spinny2
		Type	Posit
		At	root
		Geom	Centerdummy
		Bhvr	Behaviors/Splat/spinny.bhvr
	End

End

Condition	

	Event
		EName 	arrow1
		Type	start
		At	spinny
		Altpiv	1

		Bhvr	Behaviors/Splat/DarkOilLarge.bhvr
		splat	BlurredOilSplatDark.tga
	
	
	End

End

Condition	

	Event
		EName 	arrow2
		Type	start
		At	spinny2
		Altpiv	1

		Bhvr	Behaviors/Splat/DarkOilLarge.bhvr
		splat	BlurredOilSplatDark.tga


	End

End

################################################################3

Condition
	Repeat	2

	Event
		EName 	arrow
		Type	start
		At	Root

		Bhvr	Behaviors/Splat/DarkOilSmall.bhvr
		splat	BlurredOilSplatDark.tga
	
		LifeSpan	80
	
	End
	
	Event
		EName 	arrow
		Type	start
		At	Root

		Bhvr	Behaviors/Splat/DarkOil.bhvr
		splat	BlurredOilSplatDark.tga
	
		LifeSpan	80
	
	End
End

######################################################

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	WepR
		Sound Oilarrowhit 100 100 .82
	End
	
End

Condition
	On 	cycle
	Time 	12
	Chance	.3
	
	Event	
		Type	start
		At	Root
		
		Splat	wound01.tga
		Bhvr	Behaviors/Splat/LightOilRipples.bhvr

#		Part	:OilSlickCausticWaves.part
		LifeSpan	80
	End

End

Condition
	On	Time
	Time	5

	Event	
		Type	Local
		At	WepR
		Sound mudpots_loop 100 100 .73
	End
End



Condition
	On 	cycle
	Time 	5

	Event
		EName 	arrow
		Type	start
		At	Root

		Bhvr	Behaviors/Splat/DarkOil.bhvr
		splat	BlurredOilSplatDark.tga
	
		LifeSpan	100
	
	End
	
End


################Colors##################

Condition
	On 	cycle
	Time 	7
	Chance	.3

	Event
		EName 	arrow
		Type	start
		At	spinny
		Bhvr	Behaviors/Splat/LightOil.bhvr
		Splat	OilSplatDark		#OilSlickSplat.tga
	
		LifeSpan	20
	
	End
	
End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event
		EName 	arrow
		Type	start
		At	spinny2
		Bhvr	Behaviors/Splat/LightOil2.bhvr
		Splat	OilSplatDark		#OilSlickSplat.tga
	
		LifeSpan	20
	
	End
	
End

################Small oil splats##################

Condition
	On 	cycle
	Time 	2
	Chance	.3

	Event
		EName 	arrow
		Type	start
		At	Root

		Bhvr	Behaviors/Splat/DarkOilSmall.bhvr
		splat	BlurredOilSplatDark.tga
	
		LifeSpan	100
	
	End
	
End

End

		