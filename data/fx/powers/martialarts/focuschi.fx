#########################################################
##	Focus Chi
FxInfo

LifeSpan	100

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Chest
End

Lifespan 100
##################################



Condition
	On 	Time
	Time 	20



	Event
		
		Type	Local
		At	chest
		Sound chi2 70 70 .36
	End
End



Condition
	On	Time
	Time	0
		
	Event	
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	:MAChiShards01.part
		pmagnet	Chest
		
		Lifespan 65
	End

End

Condition
	On	Time
	Time	30
		
	Event	
		ENAME	Star01
		Type	Local
		At	Chest
		part1	:MAChiStar01.part
		pmagnet	Chest
		Lifespan 45
	End

End

Condition
	On	Time
	Time	40
		
	Event	
		ENAME	Star02
		Type	Local
		At	Chest
		part1	:MAChiStar02.part
		pmagnet	Chest
		Lifespan 38
	End

End

Condition
	On	Time
	Time	30
		
	Event	
		ENAME	Star03
		Type	Local
		At	Chest
		part1	:MAChiStar03.part
		pmagnet	Chest
		Lifespan 45
	End

End

Condition
	On	Time
	Time	10
		
	Event	
		ENAME	GlowFirst
		Type	Local
		At	Chest
		part1	:MAChiGlow03.part
		
	
	End

End


Condition
	On	Time
	Time	30
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MAChiGlow02.part
		
	
	End

End

Condition
	On	Time
	Time	43
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MAChiGlow02.part
		
	
	End

End

Condition
	On	Time
	Time	55
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MAChiGlow02.part
		
	
	End

End

Condition
	On	Time
	Time	65
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MAChiGlow02.part
		
	
	End

End


Condition
	On	Time
	Time	30
		
	Event	
		ENAME	Rays
		Type	Local
		At	Chest
		part1	:MAHitRays01.part
		
	
	End

End

Condition
	On	Time
	Time	43
		
	Event	
		ENAME	Rays
		Type	Local
		At	Chest
		part1	:MAHitRays01.part
		
	
	End

End

Condition
	On	Time
	Time	55
		
	Event	
		ENAME	Rays
		Type	Local
		At	Chest
		part1	:MAHitRays01.part
		
	
	End

End

Condition
	On	Time
	Time	65
		
	Event	
		ENAME	Rays
		Type	Local
		At	Chest
		part1	:MAHitRays01.part
		
	
	End

End


Condition
	On	Time
	Time	23
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MAChiRing01.part
		
	
	End

End

Condition
	On	Time
	Time	35
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MAChiRing01.part
		
	
	End

End

Condition
	On	Time
	Time	48
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MAChiRing01.part
		
	
	End

End

Condition
	On	Time
	Time	58
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MAChiRing01.part
		
	
	End

End

Condition
	On	Time
	Time	70
		
	Event	
		ENAME	GlowLast
		Type	Local
		At	Chest
		part1	:MAChiGlow03.part
		
	
	End

End




End				