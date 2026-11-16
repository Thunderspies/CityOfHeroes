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
	Event
		EName 	Prime 
		Type	start
		At	WepL
		Bhvr	:TitanRocket1.bhvr
		Magnet	Target
		LookAt	Target
	End
	#######################################
	Event
		EName 	Offset 
		Type	local
		At	Prime
		Bhvr	:TitanRocketOffset.bhvr
		#Part	:testrocket.part
	End

	Event
		EName 	Offset2 
		Type	local
		At	Offset
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:testrocket.part
	End

	Event
		EName 	Offset3 
		Type	local
		At	Offset2
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:testrocket.part
	End
	#############################################
	Event
		EName 	Offset4 
		Type	local
		At	Prime
		Bhvr	:TitanRocketOffset.bhvr
		#Part	:testrocket.part
	End

	Event
		EName 	Offset5 
		Type	local
		At	Offset4
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:testrocket.part

	End

	Event
		EName 	Offset6 
		Type	local
		At	Offset5
		Bhvr	:TitanRocketOffset2.bhvr
		Part	:testrocket.part
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