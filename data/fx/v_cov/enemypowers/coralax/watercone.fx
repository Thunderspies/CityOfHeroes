#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	60

###########################################################
##SOUND
#################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound octopusSplash1 100 100 .89
		Sound octopusSplash2 100 100 .9
	End

End

#############################################################################################

Condition
	On	Time
	Time	15
	

	Event
		Type	local
		At	root

		Part	:SplashbodyCore.part
		Part	:SplashbodyCore2.part
		Part	:SplashbodyMist.part
		Part	:SplashbodyCore3.part
		Part	:SplashBubbles.part
		Part	:SplashBubblesSmall.part

				
	End

End


End			