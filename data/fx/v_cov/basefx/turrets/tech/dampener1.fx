#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180



###########################################################


Condition
	On	Time
	Time	0
	
	Event
		EName 	Hand1
		Type	Local 
		At	wepR
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		
		Sound nightFall2 100.0 100.0 .85
		
		PMagnet	handr
		LifeSpan	50
	End

	Event
		EName 	Hand3
		Type	Local
		At	wepR
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
				
		PMagnet	wepR
		LifeSpan	50
	End


End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand2
		Type	Local 
		At	wepL
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		
		PMagnet	handL
		LifeSpan	50
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	wepL
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		
		PMagnet	wepL
		LifeSpan	50
	End

End

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	WepL
		Part1	POWERS\DarknessControl\DarknessRapidTenticles.part
		Part4	POWERS\DarknessControl\DarknessRapidTenticlesCore.part
		Part2	POWERS\DarknessControl\DarknessRapidCloud.part
		Part3	POWERS\DarknessControl\DarknessRapidCloud2.part
		#Part5	POWERS\DarknessControl\SmallTenticlesHead.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End


End


Condition
	On	Time
	Time	30


	Event
		EName	Prime1
		Type	start
		At	WepR

		Part1	POWERS\DarknessControl\DarknessRapidTenticles.part
		Part4	POWERS\DarknessControl\DarknessRapidTenticlesCore.part
		Part2	POWERS\DarknessControl\DarknessRapidCloud.part
		Part3	POWERS\DarknessControl\DarknessRapidCloud2.part
		#Part5	POWERS\DarknessControl\SmallTenticlesHead.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

End



Condition
	On 	Time
	Time	40

	Event
		EName 	HandGlowRight1
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy
		

	End

End

Condition
	On 	Time
	Time	60

	Event
		EName 	HandGlowRight2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowRight22
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft22
		Type	Destroy
		

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
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

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
		

	End


End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
		

	End

End


End			