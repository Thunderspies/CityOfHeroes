#########################################################
##	
##

FxInfo

LifeSpan  190

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	knifehand
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Knife_2
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	CenterDummy
		
		Bhvr	Behaviors\knifeSpin.bhvr	
		Magnet	Target
		LookAt	Target
		Sound throw3 100.0 100.0 .8

	End

	Event
		EName 	knife
		Type	local
		At	Prime
		Altpiv	1
		Geom	GEO_KnifeThrow
		
	End

End

##################################
Condition
	On 	Time
	Time 	16
	Event
		Ename 	knifehand
		Type	Destroy
	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End


End				