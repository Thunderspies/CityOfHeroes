#########################################################
##	Villain Base Portal
#########################################################

FxInfo

Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
				
		Bhvr	Behaviors/UXB_Fade_UP.bhvr
		Geom	rikti_UXB_Red

	End
	
	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga

	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName	crack
		Type	start
		At	root

		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End

End
Condition
	On 	Time
	Time 	6

	Event
		EName	crack
		Type	start
		At	root

		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga

	End

End

Condition
	On 	Time
	Time 	8

	Event
		EName	crack
		Type	start
		At	root

		Bhvr	:StalagmiteCracks.bhvr
		Splat	LightningTest2.tga


	End

##########################################################

	Event
		EName 	lights
		Type	Start
		At	Root
		
		Geom	UXB_lightHookup

	End
	
	Event
		Type	Start
		At	lights
		Altpiv	1
		
		Part	:SmallBlinkingLightCore.part
		Part	:SmallBlinkingLightsRed.part

	End
	
	Event
		Type	Start
		At	lights
		Altpiv	2
		
		Part	:SmallBlinkingLightCore.part
		Part	:SmallBlinkingLightsRed2.part

	End
	
End

End