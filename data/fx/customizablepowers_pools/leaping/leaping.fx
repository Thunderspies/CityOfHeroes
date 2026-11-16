#############################################################
## Leaping.fx
#############################################################

FxInfo

Lifespan 500

###############################

Input
	InpName	FootR
End
Input
	InpName	FootL
End
Input
	InpName	WepR
End
Input
	InpName	WepL
End
Input
	InpName	Hips
End
Input
	InpName	Root
End
##################################

Condition
	On	Time
	Time	0


	Event
		Type	Posit
		At	FootR
		Sound Superleap_loop 50.0 50.0 .5
		bhvr	behaviors\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End
End


Condition
	On	Time
	Time	0


	Event
		Type	Posit
		At	FootR
		Sound whoosh7 50.0 50.0 .49
	End
End


Condition
	On	Time
	Time	0


	Event
		Type	Posit
		At	FootR
		Part	:LeapingCloud.part
		Part	:LeapingCloudSteam.part
		Part	:LeapingSteam.part
		Part	:LeapingCloudSteam2.part
		Part	:LeapingMist.part
	End

	Event
		Type	Posit
		At	FootL
		Part	:LeapingCloud.part
		Part	:LeapingCloudSteam.part
		Part	:LeapingSteam.part
		Part	:LeapingCloudSteam2.part
		Part	:LeapingMist.part
	End


	Event
		Type	Local
		At	Hips
		Part	:LeapingCircleBase.part
	End


	Event
		Type	Posit
		At	WepL
		Part	:LeapingCloud.part
		Part	:LeapingCloudSteam.part
		Part	:LeapingSteam.part
		Part	:LeapingCloudSteam2.part
		Part	:LeapingMist.part
	End

	Event
		Type	Posit
		At	WepR
		Part	:LeapingCloud.part
		Part	:LeapingCloudSteam.part
		Part	:LeapingSteam.part
		Part	:LeapingCloudSteam2.part
		Part	:LeapingMist.part
	End

End

End