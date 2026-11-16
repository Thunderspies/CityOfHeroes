#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	240


Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		bhvr	:ArrowRain.bhvr
		Geom	NormalArrow	
	End


End

Condition
	On	PrimeHit
		
		
	
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			