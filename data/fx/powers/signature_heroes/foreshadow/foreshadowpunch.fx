#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	45
######################################################################

Condition	
	On	time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound WisdomBlast 110 110 .94
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmR
		
		LifeSpan	17
	
	End
	
	Event
		EName	Streak1
		Type	local
		At	UARML
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	LarmL

		LifeSpan	17
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepR

		LifeSpan	17
	End
	
	Event
		EName	Streak1
		Type	local
		At	LarmL
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepL

		LifeSpan	17
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepR

		LifeSpan	17
	End
	
	Event
		EName	Streak1
		Type	local
		At	WepL
		Part	:StrengthTendrils.part
		Part	:StrengthTendrilsInvert.part
		Part	:Glows.part
		Part	:StrengthBodyGlows.part
		Part	:StrengthBodyGlows1.part
		Part	:StrengthBodyGlows3.part
		Part	:StrengthBodyGlows4.part
		PMagnet	WepL

		LifeSpan	17
	End
End

###########################
##Geom  
#####################

Condition
	On 	Time
	Time 	16

	Event
		EName	Streak1
		Type	start
		At	BendMystic
		
		BhvrOverride

			PositionOffset		0 -.5 0
		End

		Bhvr	Behaviors/ForeShadowFade.bhvr
		Geom	ForeShadowBlast
		
		LifeSpan	1
	End

End

###########################
##Blast
#####################


Condition
	On 	Time
	Time	13

	Event
		EName	Prime
		Type	start
		At	wepR
		
		BhvrOverride
			PositionOffset		0 2 0
		End


		Bhvr	Behaviors/ForeShadowBlurProjectiles.bhvr
		
		Part	:BlastTendrilsInvert.part
		Part	:BlastTendrils.part
		Part	:BlastTendrilsInvertBright.part
		Part	:BlastTendrilsBright.part

		Magnet	Target

	End


	Event

		EName	Prime1
		Type	local
		At	Prime
		
		BhvrOverride
			PyrRotate	95 0 0
		End

		Part	:SmokeRingBlast.part

		
	End

End

Condition
	On	Time
	Time	15

	
	Event

		EName	Prime2
		Type	local
		At	Prime
		
		BhvrOverride
			PyrRotate	95 0 0
		End

		Part	:SmokeRingBlast2.part

		
	End
End


Condition
	On	Time
	Time	15

	
	Event

		EName	Prime3
		Type	local
		At	Prime
		
		BhvrOverride
			PyrRotate	95 0 0
		End

		Part	:SmokeRingBlast3.part

		
	End
End

###########################
##Rings
#####################

Condition
	On 	Time
	Time 	11

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.35 0
			PyrRotate	0 0 145
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		
	End

End

Condition
	On 	Time
	Time 	12

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 145
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.45 0
			PyrRotate	0 0 145
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		LifeSpan	1
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName	Streak1
		Type	local
		At	WepR
		
		BhvrOverride
			PositionOffset		0 -.5 0
			PyrRotate	0 0 145
		End

		Part2	:StrengthRingKO.part
		Part2	:StrengthRingBurst.part
		LifeSpan	1
	End

End


Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End

	Event
		EName 	Prime1
		Type	Destroy
		
	End
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End
	
	Event
		EName 	Prime3
		Type	Destroy
		
	End

End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

End

