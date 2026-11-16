#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	Inpname	root
End

Input  
	InpName	Hips
End    

LifeSpan	80
#########################################################

Condition
	On 	Time
	Time 	0

	

	Event
		ENAME   Tendrills
		Type	Local 
		At	Hips
		Part1	:DarkBlastHitTendril2.part
		Part2	:DarkBlastHitPuddle2.part
		LifeSpan	20
	End

End

End
########################################################

			