#########################################################
##	chill touch hit
FxInfo


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
		bhvr	behaviors/soundFade2.bhvr
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
		part1	:LeapingCloud.part
		part2	:LeapingCloudSteam.part
		part3	:LeapingSteam.part
		part4	:LeapingCloudSteam2.part
		Part5	:LeapingMist.part

	End

	Event	
		Type	Posit
		At	FootL
		part1	:LeapingCloud.part
		part2	:LeapingCloudSteam.part
		part3	:LeapingSteam.part
		part4	:LeapingCloudSteam2.part
		Part5	:LeapingMist.part
	End
	
		
	Event	
		Type	Local
		At	Hips
		part1	:LeapingCircleBase.part
	End


	Event	
		Type	Posit
		At	WepL
		part1	:LeapingCloud.part
		part2	:LeapingCloudSteam.part
		part3	:LeapingSteam.part
		part4	:LeapingCloudSteam2.part
		Part5	:LeapingMist.part
	End

	Event	
		Type	Posit
		At	WepR
		part1	:LeapingCloud.part
		part2	:LeapingCloudSteam.part
		part3	:LeapingSteam.part
		part4	:LeapingCloudSteam2.part
		Part5	:LeapingMist.part
	End


End


End				