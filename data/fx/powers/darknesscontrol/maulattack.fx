#########################################################
##	template

FxInfo

Condition
	On	Time
	Time	0
	

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LArmL
		LifeSpan	45

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepL
		LifeSpan	45
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LArmR
		LifeSpan	45

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepR
		LifeSpan	45
	End

	Event
		EName	15
		Type	Local
		At	WepL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepR
		LifeSpan	45
	End

	Event
		EName	16
		Type	Local
		At	WepR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepR

		LifeSpan	45
	End

End

#########################################################
##fLURRY OF FISTS ANIM
###########################################



Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	root
		Anim	MblurShadowFighting

	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	root
		Anim	MblurShadowFighting
	End		
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	root
		Anim	MblurShadowFighting
		Sound Flurry 50 50 .75
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	root
		Anim	MblurShadowFighting
	End
End

Condition
	On 	Time
	Time 	90
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		