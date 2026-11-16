#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	20

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
		Part1	CustomizeablePowers\Necromancy\DarkBlastHitTendril2.part
		Part2	CustomizeablePowers\Necromancy\DarkBlastHitPuddle2.part
		
	End

End

End	


			