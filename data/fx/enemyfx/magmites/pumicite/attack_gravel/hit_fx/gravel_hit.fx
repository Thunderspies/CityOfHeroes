#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On	Time
	Time	0
			
	Event
		
		Type	local
		At	Chest
		Part	:GravelHit_Dust.part
		Part	:Gravel_HitRocks_01.part
		Part	:Gravel_HitRocks_Dark.part
		Sound pumicehit 80 80 .9
		Lifespan	30
	End

	
End

Condition
	On	Time
	Time	5
			
	Event
		
		Type	local
		At	Chest
		Part	:GravelHit_Dust.part
		Part	:Gravel_HitRocks_01.part
		Part	:Gravel_HitRocks_Dark.part
		Lifespan	30
	End

	
End

Condition
	On	Time
	Time	11
			
	Event
		
		Type	local
		At	Chest
		Part	:GravelHit_Dust.part
		Part	:Gravel_HitRocks_01.part
		Part	:Gravel_HitRocks_Dark.part
		Lifespan	30
	End

	
End

Condition
	On	Time
	Time	15
			
	Event
		
		Type	local
		At	Chest
		Part	:GravelHit_Dust.part
		Part	:Gravel_HitRocks_01.part
		Part	:Gravel_HitRocks_Dark.part
		Lifespan	30
	End

	
End

End	


			