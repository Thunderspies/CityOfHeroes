#########################################################
##	Succubus Seduction Debuff Attack
#########################################################  

FxInfo

Lifespan	240

Condition
	On 	Time
	Time 	0

	Event
		EName 	Glow
		Type	Local 		
		At	WepL
		Part1	:seductionGlow.part
		Part2	:seductionCore.part
		Part3	:seductionTrail.part
		Part4	:seductionSparkles.part
		Sound seduction1 80 80 .85
		
	End

End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	HandL
		Sound Seductiontravel4 88 88 .99
	End
End

Condition
	On 	Time
	Time 	27


	Event
		EName 	Blow
		Type	Local
		At	HandL
		
		Part1	:seductionMist.part
		Part2	:seductionCloud.part
		Part3	:seductionDust.part
		

	End

	Event
		EName 	Prime
		Type	Start 
		At	Glow
		Bhvr	:seductionProjectile.bhvr
		Magnet	T_Head
		LookAt	T_Head
	End


	Event
		EName 	Glow
		Type	Destroy
	End

	Event
		EName 	Glow2
		Type	Local 		
		At	Prime
		Part1	:seductionGlow.part
		Part2	:seductionCore.part
		Part3	:seductionTrail.part
		Part4	:seductionSparkles.part
		
	End


End

Condition
	On 	Time
	Time	52

	Event
		EName 	Blow
		Type	Destroy
	End

End	


Condition
	On 	PrimeHit

	Event
		EName 	All
		Type	Destroy
	End

End	



End