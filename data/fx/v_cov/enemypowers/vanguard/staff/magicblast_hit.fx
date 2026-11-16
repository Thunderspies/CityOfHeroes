#########################################################
## Staff of the M.A.G.I. - Magic Blast Hit
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
		Part1	POWERS\DarknessControl\DarkBlastHitTendril2.part
		Part2	POWERS\DarknessControl\DarkBlastHitPuddle2.part
		
	End

End

End