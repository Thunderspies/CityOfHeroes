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
		EName 	attractor
		Type	Local 
		At	root
		bhvroverride
			PositionOffset		0 9.7 0
		end
	end


	Event
		EName 	crystal
		Type	Local 
		At	root
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
		bhvroverride
			PositionOffset		0 8.5 0
		end
		Sound nightFall2 100.0 100.0 .85
		
		PMagnet	attractor
		LifeSpan	50
	End

	Event
		EName 	Hand3
		Type	Local
		At	crystal
		Part1	POWERS\DarknessControl\DarkBlastTenticle.part
		Part3	POWERS\DarknessControl\DarkHandsEmberb.part
				
		PMagnet	crystal
		LifeSpan	50
	End


End


Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	crystal
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