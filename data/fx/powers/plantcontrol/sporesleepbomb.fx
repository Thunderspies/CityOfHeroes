#########################################################
##	
##

FxInfo
LifeSpan  200


Input  
	InpName	Origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	WepR
End

##################################



Condition
	On 	Time
	Time 	31

	Event
		EName 	Prime
		Type	start 
		At	WepR
		#Geom	Puffball
		Geom	Puffball02
		bhvr	:PuffBallLobbprojectile.bhvr
		Part	:SporeDustProjectile.part
		Part	:SporeDustProjectileMist.part
		Part	:SporeDustGlows.part
		Magnet	T_Chest
		LookAt	T_Chest
		Sound mushroomtoss 60 60 .7

	End

	
End

##################################


Condition
	On 	PrimeHit
	
	Event
		Type	start 
		At	T_Root
		Part	:SporeDustScatter.part
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


End				