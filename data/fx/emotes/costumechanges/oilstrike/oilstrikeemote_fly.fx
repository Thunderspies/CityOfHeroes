#############################################################
## OilStrikeEmote.fx
#############################################################

FxInfo
Lifespan 223

#############################################################

Input
	InpName	root
End

#############################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound SP_OilStrike_01 100 100 .8
	End
End

Condition
	On	Time
	Time	23

	Event
		Type	Start
		At	Root
		Part	:Geyser_Core.part
		Part	:Geyser_Core_Highlight.part
		Part	:Geyser_Core_Highlight2.part
		Part	:Geyser_Droplets.part
		Part	:Geyser_Strands.part
		Part	:Geyser_Strands_Scrolling.part

		Lifespan	50
	End

	Event
		Type	Start
		At	Root
		Part	:Geyser_Ground_Splash.part
		Part	:Geyser_Ground_Splash_Fine.part

		Lifespan	90
	End
End


#############################################################

End