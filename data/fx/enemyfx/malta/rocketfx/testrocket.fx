#########################################################
##	
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

LifeSpan  100

##################################

Condition
	On 	Time
	Time 	0


	Event
		EName 	Prime 
		Type	start
		At	WepL
		#Altpiv 2
		#Geom	
		Bhvr	:TitanRocket1.bhvr
		Part	:TestRocket.part
		Part	:TestRocket.part
		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	Time
	Time 	6


	Event
		EName 	Prime1 
		Type	start
		At	WepR
		#Altpiv 2
		#Geom	
		Bhvr	:TitanRocket1.bhvr
		Part	:TestRocket.part
		Part	:TestRocket.part
		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	Time
	Time 	12


	Event
		EName 	Prime2 
		Type	start
		At	WepL
		#Altpiv 2
		#Geom	
		Bhvr	:TitanRocket1.bhvr
		Part	:TestRocket.part
		Part	:TestRocket.part
		Magnet	Target
		LookAt	Target
	End

End

Condition
	On 	Time
	Time 	18


	Event
		EName 	Prime3 
		Type	start
		At	WepR
		#Altpiv 2
		#Geom	
		Bhvr	:TitanRocket1.bhvr
		Part	:TestRocket.part
		Part	:TestRocket.part
		Magnet	Target
		LookAt	Target
	End

End



##################################

Condition
	On	PrimeHit
	Time	0

	Event
		Type	Local
		At	Origin
		#Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	PrimeHit
	Time 	0

	Event
		EName	Burst
		Type	Start
		At	T_Chest
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		
		Lifespan 25

		
	End
End

Condition
	On 	PrimeHit
	Time 	5

	Event
		EName	Smoke
		Type	Start
		At	T_Chest
		Part	:Smoke2.part
		Lifespan 5

	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Explosion
		Type	Destroy
	End

End

End				