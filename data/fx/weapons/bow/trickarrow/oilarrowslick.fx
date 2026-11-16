#############################################################
## Trick Arrow - Oil Slick
#############################################################

FxInfo

#############################################################

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
	On	Time
	Time	5

	Event	
		Type	Local
		At	WepR
		Sound mudpots_loop 100 100 .73
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

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

#############################################################

Condition	
	On 	Time
	Time 	0
	Repeat	2

	Event
		EName 	arrow1
		Type	start
		At	Root
		Bhvr	Behaviors/Splat/DarkOilLarge.bhvr
		splat	SleetIceSlick.tga #StormCloudGlow.tga #BlurredOilSplatDark.tga
	End
End

#Condition
#	On 	Time
#	Time 	0
#	Repeat	2
#	
#	Event
#		EName 	arrow
#		Type	start
#		At	Root
#		Bhvr	Behaviors/Splat/DarkOil.bhvr
#		splat	BlurredOilSplatDark.tga
#		LifeSpan	80
#	End
#End

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		EName 	arrow
		Type	start
		At	Root
		Bhvr	Behaviors/Splat/DarkOilSmall.bhvr
		splat	BlurredOilSplatDark.tga
		#LifeSpan	100
	End
End

## RAINBOW SHEEN ####################################################

#Condition
#	On 	Time
#	Time 	0
#	Repeat	8
#	
#	Event	
#		EName 	RainbowRipples
#		Type	Start
#		At	Root
#		Splat	wound01.tga
#		Bhvr	Behaviors/Splat/LightOilRipples.bhvr
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#	Repeat	8
#
#	Event
#		EName 	RainbowSheen
#		Type	Start
#		At	Root
#		Bhvr	Behaviors/Splat/LightOil2.bhvr
#		Splat	OilSplatDark
#	End
#End

#############################################################

End		