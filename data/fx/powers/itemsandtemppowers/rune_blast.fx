#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	300
   

Condition
	On 	Time
	Time 	10
	
	Event
		Type	local
		At	WepL
		
		#Part	:HandTendrils.part
		Part	:Rune1a.part
		Sound runecharge 80 80 .9
		LifeSpan	55		
		
	End

End    

Condition
	On 	Time
	Time 	20
	
	Event
		Type	local
		At	WepL
		
		Part	:HandTendrils.part
		#Part	:Rune1a.part
		
		LifeSpan	50		
		
	End

End  

Condition
	On 	Time
	Time 	65
	
	
	Event
		Type	local
		At	WepL
		
		Part	:HandTendrilsBurst.part
		Sound RuneBlast1 80 80 .9
		LifeSpan	20		
		
	End

	Event
		Ename	Prime
		Type	start
		At	WepL
		
		Bhvr	Behaviors/Projectile.bhvr
		Part	:Rune2a.part
		Part	:Rune2a.part
		Part	:RuneSmoke.part
		Part	:RuneSparklies.part
		Part	:Tendrils.part
		
		LifeSpan	60		
		Magnet	Target
		
	End

End	

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End
End

End