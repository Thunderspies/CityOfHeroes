################x#########################################
##	chill touch hit
FxInfo

Input  
	InpName	head
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Chest
End

LifeSpan	75

############################################################


Condition
	On 	Time
	Time 	30

	Event	

		ENAME	skull
		Type	local
		At	head
		Anim	Fx_Skull
		bhvr	behaviors/SkullAlpha.bhvr
		Sound aura4 80 80 .8
		
	End

End

Condition
	On 	Time
	Time 	20

	Event	

		ENAME	HandR
		Type	local
		At	WepR

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part

	End

	Event	

		ENAME	HandL
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
	End
	
	
	Event
		EName 	Orb
		Type	Local 
		At	head
		
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		
	End

End	

Condition
	On 	Time
	Time 	30

	Event	

		ENAME	geoEyes
		Type	local
		At	head
		Geom	HeadEyes
		bhvr	behaviors/SkullAlpha.bhvr
		
	End

	Event	

		ENAME	LeftEye
		Type	local
		At	geoEyes
		altpiv	1
		Part1	:DaemonEyes.part
		Part2	:DaemonPupil.part
	End

	Event	

		ENAME	RightEye
		Type	local
		At	geoEyes
		altpiv	2

		Part1	:DaemonEyes.part
		Part2	:DaemonPupil.part
	End

End


Condition
	On 	Time
	Time	64
	Event
		EName 	Skull
		Type	Destroy

	End

End


Condition
	On 	Time
	Time	62

	Event
		EName 	rightEye
		Type	Destroy

	End

	Event
		EName 	leftEye
		Type	Destroy

	End

End

Condition
	On 	Time
	Time	55
	Event
		EName 	HandL
		Type	Destroy

	End

	Event
		EName 	HandR
		Type	Destroy

	End

	Event
		EName 	orb
		Type	Destroy

	End

End

End			