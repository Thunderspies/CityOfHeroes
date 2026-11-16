#############################################################
## Geyser_Hydrant.fx
#############################################################

FxInfo
Lifespan 360 #180

#############################################################

Input
	InpName	root
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin

		Part	:Top_Spray_Geyser.part
		Part	:Geyser_Droplets_Top.part
		Part	:Geyser_Core.part
		Part	:Geyser_Core_Highlight.part
	#	Sound FireHydrant_Blow1 200 200 .35
		Lifespan	360
	End


#############################################################

End