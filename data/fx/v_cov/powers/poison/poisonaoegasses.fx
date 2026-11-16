#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100


#########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	WepL
		
		Part	:NoxiousGassHand.part
		Part	:DarkNoxiousGassHand.part
		Part	:NoxiousGassHandBubble.part
		Part	:NoxiousGassHandStars.part
		Part	:NoxiousGassHandGlows.part
		Sound PoisonGasSpew 80 80 .7
		LifeSpan	30

	End
		
End

Condition
	On 	Time
	Time 	15

	Event
		EName	Prime
		Type	start
		At	WepL

		Bhvr	:Quillprojectile1.bhvr
		Magnet	T_Root
		LookAt	T_Root
		
		Part	:QuillTrailBlack.part
		Part	:QuillTrail.part
		Part	:QuillTrailLight.part

		Part	:QuillTrailStreak.part
	#		Part2	weaponFx/QuillTrailSpecks.part
#		Part3	weaponFx/QuillBubblesTrail.part
		Part5	:QuillBallzTrail.part
		Part5	:QuillBallzTrailBlack.part

	End
	
End

Condition
	On 	PrimeHit


	Event
		Type	local
		At	T_Root
		
		Part	:AOETrailBlack.part
		Part	:AOETrail.part
		Part	:AOETrailLight.part

		Part	:AOETrailStreak.part
		Part2	:AOEYellowSmallBallzTrail.part
		Part3	:AOEYellowSmallBubble.part
		Part	:AOEYellowSmallstars.part
		Part5	:AOEBallzTrail.part
		Part5	:AOEBallzTrailBlack.part
		
		LifeSpan	100
	End

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				