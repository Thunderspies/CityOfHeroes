#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	10

Input  
	InpName	Hips
End    

#########################################################

Condition
	On 	Time
	Time 	0

	

	Event
		ENAME   Tendrills
		Type	Local 
		At	Hips
		Part1	POWERS/DarknessControl/DarkBlastHitTendril2.part
		Part2	POWERS/DarknessControl/DarkBlastHitPuddle2.part
		Sound DirtExplowidow2 80 80 .8
		Lifespan	7
		
	End

End

Condition
	On	Time
	Time	1

	Event
		EName 	Explode 
		Type	Start
		At	Waist
		Part	:soulFlash1.part
		Part	:soulExplode2.part		
		Part	:soulSparks2.part
		Part	:soulCloud2.part

		Lifespan	1
	End


End

End	


			